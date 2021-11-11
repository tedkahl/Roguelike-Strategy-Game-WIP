#pragma once
#include <array>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <functional>
#include "assert.h"
#include "EntityUpdate.h"
#include "DCSortable.h"
//template<typename T>
//concept accessible = requires(const std::remove_reference_t<T>& t1, const std::remove_reference_t<T>& t2) {
//	{t1 < t2}->std::boolean-testable;
//	T::SortType;
//};
using std::cout;
using std::endl;
static const size_t S_MAX = 3000;
class DrawComponent;

template<typename T>
//requires accessible<T>
class SortedDManager {
protected:
	static const size_t SWAPSIZE = 100;
	std::array<T, S_MAX> data;
	std::array<T, SWAPSIZE> swap;
	std::vector<unsigned> batches;
	std::vector<unsigned> deactivated;
	unsigned active_ = 0;
	unsigned unsorted_ = 0;

public:
	T* get(T::SortType a);
	//unsigned activate();

	inline int addBatch();
	void deleteBatch(unsigned batch);
	template<typename ...T_>
	T* declareNew(T_&&... args);

	template<typename ...T_>
	T* declareNewBatched(T_&&... args);

	//bool unsorted() const { return unsorted_ > 0; }
	void sort();
	void sortNew();
	void sortDeleted();
	void fixChangedVal(T* to_fix);
	void deactivate(T::SortType to_deactivate);

	template<typename Sortable>
	void updateAll(sf::Time current, T::SortType base, Sortable& s, EntityUpdate& u);
	void fixAll(std::array<T, S_MAX>::iterator first, std::array<T, S_MAX>::iterator last);

	inline size_t active() const { return active_; }
	typedef std::array<T, S_MAX>::iterator iterator;
	iterator begin() { return data.begin(); }
	iterator end() { return data.begin() + active_; }
};
template<typename T>
//requires accessible<T>
T* SortedDManager<T>::get(T::SortType a) {
	if (unsorted_ > 0) sort();
	int start = 0;
	int end = active_;
	int mid;
	while (start <= end) {
		mid = (end + start) / 2;
		if (data[mid] == a) return &data[mid];
		else if (data[mid] < a) start = mid + 1;
		else end = mid - 1;
	}
	assert(false); //should always find
	return nullptr;
}

template<typename T>
//requires accessible<T>
void SortedDManager<T>::sort() {
	//cout << "sorting" << endl;
	if (!deactivated.empty()) sortDeleted();
	if (unsorted_ > 0 && unsorted_ <= SWAPSIZE) sortNew();
	if (unsorted_ > SWAPSIZE) std::sort(begin(), end()); //else quicksort
	unsorted_ = 0;
}

template<typename T>
//requires accessible<T>
void SortedDManager<T>::sortNew()//if just a few elements were added during normal gameplay, sort in O(n)
{
	std::copy(end() - unsorted_, end(), swap.begin());
	std::sort(swap.begin(), swap.begin() + unsorted_);
	unsigned i = active_ - 1;
	while (unsorted_ > 0) {
		if (data[i - unsorted_] < swap[unsorted_ - 1]) {
			data[i] = swap[unsorted_ - 1];
			--unsorted_;
		}
		else data[i] = data[i - unsorted_];
		--i;
	}
}

template<typename T>
//requires accessible<T>
void SortedDManager<T>::sortDeleted()
{
	std::sort(deactivated.begin(), deactivated.end());

	unsigned count = 0;
	for (unsigned i = 0;i < active_ - count;i++) {
		if (count < deactivated.size() && i + count == deactivated[count]) {
			count++;
			//std::cout << "deleted element found at " << i + count << std::endl;
		}
		data[i] = data[i + count];
	}
	active_ -= deactivated.size();
	deactivated.clear();
}

//changes in zvalue should be small, so just bubble it up or down here
template<typename T>
void SortedDManager<T>::fixChangedVal(T* to_fix) {
	int index = to_fix - data.data();
	while (index > 0 && data[index] < data[index - 1]) {
		std::swap(data[index], data[index - 1]);
		index--;
	}

	while (index < active_ - 1 && data[index + 1] < data[index]) {
		std::swap(data[index], data[index + 1]);
		index++;
	}
}

template<typename T>
//requires accessible<T>
int SortedDManager<T>::addBatch()
{
	int newbatch = batches.empty() ? 0 : batches.back() + 1;
	batches.push_back(newbatch);
	return newbatch;
}

template<typename T>
//requires accessible<T>
void SortedDManager<T>::deleteBatch(unsigned batch)
{
	unsigned count = 0;
	for (unsigned i = 0;i < active_ - count;i++) {
		while (data[i + count].batch() == batch) count++;
		if (i + count < active_) data[i] = data[i + count];
	}
	active_ -= count;

	auto it = std::lower_bound(batches.begin(), batches.end(), batch);
	assert(it != batches.end());
	batches.erase(it);
}

template<typename T>
//requires accessible<T>
template<typename ...T_>
T* SortedDManager<T>::declareNew(T_&&... args)
{
	active_++;
	unsorted_++;
	assert(active_ <= S_MAX);
	data[active_ - 1].set(std::forward<T_>(args)...);
	return &data[active_ - 1];
}
//a less than comparison which returns false if a is less than, but close enough to be associated with, b
template<typename T>
static bool lt_associated(const T& first, const DCSortable::SortType& b) {
	//assert(std::get<4>(a) == 50);
	DCSortable::SortType a = first;
	if (std::get<0>(a) == std::get<0>(b) && std::get<1>(a) == std::get<1>(b) && std::get<2>(a) == std::get<2>(b) && std::get<3>(a) == std::get<3>(b)) {
		return std::get<4>(b) - std::get<4>(a) >= 25;
	}
	else return first < b;
}

template<typename T>
template<typename Sortable>
void SortedDManager<T>::updateAll(sf::Time current, T::SortType base, Sortable& s, EntityUpdate& u) {
	static_assert(std::same_as<T, DrawComponent>);
	auto first = std::lower_bound(begin(), end(), base, lt_associated<T>);
	auto dup = first;
	bool dirty = false;
	while (lt_associated(base, (*first).sortVal())) {
		dirty = dirty || first->update(current, u, s);
		first++;
	}
	if (dirty) fixAll(dup, first);
}

//orig 10 20 30 40 50 60 70 80 90 100
//update 10 20 30 40 15 16 17 80 90 100
//copy to temp 15, 16, 17
//iterate down 10 20 30 40 20 30 40 80 90 100
//copy from temp 10 15 16 17 20 30 40 80 90 100
template<typename T>
void SortedDManager<T>::fixAll(std::array<T, S_MAX>::iterator first, std::array<T, S_MAX>::iterator last) {
	if (first != begin() && *first < *(first - 1)) {
		std::vector<T> copies(last - first + 1);
		std::copy(first, last, copies.begin());
		first--;
		while (copies.back() < (*first) && first > begin() + copies.size()) {
			*last = *first;
			last--;
			first--;
		}
		std::copy(copies.begin(), copies.end(), first - copies.size());
	}
	else if (last != end() && *(last + 1) < *last) {
		std::vector<T> copies(last - first);
		std::copy(first, last, copies.begin());
		first++;
		while (*last < *copies.begin() && last < end() - copies.size()) {
			*first = *last;
			last++;
			first++;
		}

	}
}

template<typename T>
//requires accessible<T>
template<typename ...T_>
T* SortedDManager<T>::declareNewBatched(T_&&... args) //add new and mark as latest batch
{
	assert(!batches.empty());
	active_++;
	unsorted_++;
	assert(active_ <= S_MAX);
	data[active_ - 1].set(std::forward<T_>(args)...);
	return &data[active_ - 1];
}


template<typename T>
//requires accessible<T>
void SortedDManager<T>::deactivate(T::SortType to_deactivate) {
	unsigned index = get(to_deactivate) - data.data();
	std::cout << "deactivated element at index " << index;
	assert(index < active_);
	deactivated.push_back(index);
}

typedef SortedDManager<DrawComponent> DCManager;
