#pragma once
#include <array>
#include <vector>
#include <string>
#include <algorithm>
#include "assert.h"
static const size_t MAX = 2000;
template<typename T, typename Accessor>
class SortedDManager {
private:
	static const size_t SWAPSIZE = 200;
	std::array<T, MAX> data;
	std::array<T, SWAPSIZE> swap;
	std::vector<unsigned> batches;
	std::vector<unsigned> deactivated;
	unsigned active_ = 0;
	unsigned unsorted = 0;
public:
	T* firstInvalidPtr() { return &data[active_]; }
	T* get(Accessor a);
	//unsigned activate();

	inline void addBatch();
	void deleteBatch(int batch);
	template<typename ...T_>
	Accessor declareNew(T_... args);

	template<typename ...T_>
	Accessor declareNewBatched(T_... args);

	void sort();
	void sortNew();
	void sortDeleted();
	void fixChangedVal(T* to_fix);
	void deactivate(unsigned index);
	inline size_t active() const { return active_; }
	typedef std::array<T, MAX>::iterator iterator;
	iterator begin() { return data.begin(); }
	iterator end() { return data.begin() + active_; }
};
template<typename T, typename Accessor>
T* SortedDManager<T, Accessor>::get(Accessor a) {
	unsigned start = 0;
	unsigned end = active_;
	unsigned mid;
	while (start <= end) {
		mid = (end - start) / 2;
		if (a == data[mid]) return &data[mid];
		else if (a < data[mid]) end = mid - 1;
		else start = mid + 1;
	}
	assert(false); //should always find
}

template<typename T, typename Accessor>
void SortedDManager<T, Accessor>::sort() {
	if (unsorted > 0 && unsorted <= SWAPSIZE) sortNew();
	if (!deactivated.empty()) sortDeleted();
	if (unsorted > SWAPSIZE) std::sort(begin(), end()); //else quicksort
	unsorted = 0;
}

template<typename T, typename Accessor>
void SortedDManager<T, Accessor>::sortNew()//if just a few elements were added during normal gameplay, sort in O(n)
{
	std::copy(end() - unsorted, end(), swap.begin());
	std::sort(swap.begin(), swap.begin() + unsorted);
	unsigned i = active_ - 1;
	while (unsorted > 0) {
		if (data[i - unsorted] < swap[unsorted - 1]) {
			data[i] = swap[unsorted - 1];
			--unsorted;
		}
		else data[i] = data[i - unsorted];
		--i;
	}
}

template<typename T, typename Accessor>
void SortedDManager<T, Accessor>::sortDeleted()
{
	std::sort(deactivated.begin(), deactivated.end());

	unsigned count = 0;
	for (unsigned i = 0;i < active_ - count;i++) {
		if (data[i + count] == deactivated[count]) count++;
		data[i] = data[i + count];
	}
	active_ -= deactivated.size();
	deactivated.clear();
}

//changes in zvalue should be small, so just bubble it up or down here
template<typename T, typename Accessor>
void SortedDManager<T, Accessor>::fixChangedVal(T* to_fix) {
	int index = to_fix - data.data();
	while (index > 0 && data[index] < data[index - 1]) {
		std::swap(data[index], data[index - 1]);
		index--;
	}

	while (index <active_ - 1 && data[index] > data[index + 1]) {
		std::swap(data[index], data[index + 1]);
		index++;
	}
}

template<typename T, typename Accessor>
void SortedDManager<T, Accessor>::addBatch()
{
	int newbatch = batches.empty() ? 0 : batches.back() + 1;
	batches.push_back(newbatch);
}

template<typename T, typename Accessor>
void SortedDManager<T, Accessor>::deleteBatch(int batch)
{
	unsigned count = 0;
	for (unsigned i = 0;i < active_ - count;i++) {
		if (data[i + count].batch() == batch) count++;
		data[i] = data[i + count];
	}
	active_ -= count;

	auto it = std::lower_bound(batches.begin(), batches.end(), batch);
	assert(it != batches.end());
	batches.erase(batch);
}

template<typename T, typename Accessor>
template<typename ...T_>
Accessor SortedDManager<T, Accessor>::declareNew(T_... args)
{
	active_++;
	unsorted++;
	assert(active_ <= MAX);
	data[active_ - 1].set(args..., active_ - 1);
	return Accessor([active_ - 1]);
}

template<typename T, typename Accessor>
template<typename ...T_>
Accessor SortedDManager<T, Accessor>::declareNewBatched(T_... args) //add new and mark as latest batch
{
	assert(!batches.empty());
	active_++;
	unsorted++;
	assert(active_ <= MAX);
	data[active_ - 1].set(args..., batches.back());
	return Accessor([active_ - 1]);
}


template<typename T, typename Accessor>
void SortedDManager<T, Accessor>::deactivate(unsigned index) {
	assert(index < active_);
	deactivated.push_back(index);

	std::swap(data[index], data[active_ - 1]);
	data[index].setIndex(index);
	data[active_ - 1].setIndex(active_ - 1);
	data[index].updatePointers(data[active_ - 1]);
	--active_;
}
