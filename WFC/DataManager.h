#pragma once
#include <array>
#include <string>
#include "assert.h"
template<typename T, size_t MAX>
class DataManager {
private:
	std::array<T, MAX> data;
	unsigned active_ = 0;
public:
	T* firstInvalidPtr() { return &data[active_]; }
	T* get(unsigned index) { return &data[index]; }
	//unsigned activate();

	template<typename ...T_>
	T* declareNew(T_... args);

	void deactivate(unsigned index);
	inline size_t active() const { return active_; }
	typedef std::array<T, MAX>::iterator iterator;
	iterator begin() { return data.begin(); }
	iterator end() { return data.begin() + active_; }
};

template<typename T, size_t MAX>
template<typename ...T_>
T* DataManager<T, MAX>::declareNew(T_... args)
{
	active_++;
	assert(active_ <= MAX);
	data[active_ - 1].set(args..., active_ - 1);
	return &data[active_ - 1];
}


template<typename T, size_t MAX>
void DataManager<T, MAX>::deactivate(unsigned index) {
	assert(index < active_);
	std::swap(data[index], data[active_ - 1]);
	data[index].setIndex(index);
	data[active_ - 1].setIndex(active_ - 1);
	data[index].updatePointers(data[active_ - 1]);
	--active_;
}
