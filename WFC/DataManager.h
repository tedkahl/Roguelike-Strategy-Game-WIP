#pragma once
#include <array>
#include <string>
#include "assert.h"
const size_t MAX = 300;
template<typename T>
class DataManager {
protected:
	std::array<T, MAX> data;
	unsigned active_ = 0;
	std::vector<int> deactivated;
public:
	T* firstInvalidPtr() { return &data[active_]; }
	T* get(unsigned index) { return &data[index]; }
	//unsigned activate();
	template<typename ...T_>
	T* declareNew(T_&&... args);
	void deactivate(unsigned index);
	inline size_t active() const { return active_; }
	void fix();
	void deactivateLater(int index);

	T& operator[](size_t i) { return data[i]; }
	typedef std::array<T, MAX>::iterator iterator;
	iterator begin() { return data.begin(); }
	iterator end() { return data.begin() + active_; }

	DataManager() = default;
	DataManager(const DataManager<T>& other) = delete;
	DataManager<T>& operator=(const DataManager<T>& other) = delete;
};

template<typename T>
template<typename ...T_>
T* DataManager<T>::declareNew(T_&&... args)
{
	active_++;
	assert(active_ <= MAX);
	data[active_ - 1].set(std::forward<T_>(args)..., active_ - 1);
	return &data[active_ - 1];
}


template<typename T>
void DataManager<T>::deactivate(unsigned index) {
	assert(index < active_);
	{
		data[index] = std::move(data[active_ - 1]);
	}
	data[index].setIndex(index);
	data[active_ - 1].setIndex(active_ - 1);
	data[index].updatePointers(data[active_ - 1]);
	--active_;
}

template<typename T>
void DataManager<T>::fix() {
	if (deactivated.size() > 0) {
		std::sort(deactivated.begin(), deactivated.end(), [](const int& a, const int& b) {return a > b;});
		for (auto& i : deactivated) {
			data[i] = std::move(data[active_ - 1]);
			data[i].updatePointers(data[active_ - 1]);
			data[i].setIndex(i);
			--active_;
		}
	}
	deactivated.clear();
}

template<typename T>
void DataManager<T>::deactivateLater(int index) {
	deactivated.push_back(index);
}
