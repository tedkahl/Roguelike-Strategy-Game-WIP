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
	bool dirty_flag = false;
public:
	T* firstInvalidPtr() { return &data[active_]; }
	T* get(unsigned index) { return &data[index]; }
	//unsigned activate();
	bool isChanged() const { return dirty_flag; }
	void setChanged(bool d) { dirty_flag = d; }
	template<typename ...T_>
	T* declareNew(T_&&... args);
	void deactivate(unsigned index);
	inline size_t active() const { return active_; }
	typedef std::array<T, MAX>::iterator iterator;
	iterator begin() { return data.begin(); }
	iterator end() { return data.begin() + active_; }
};

template<typename T>
template<typename ...T_>
T* DataManager<T>::declareNew(T_&&... args)
{
	dirty_flag = true;
	active_++;
	assert(active_ <= MAX);
	data[active_ - 1].set(std::forward<T_>(args)..., active_ - 1);
	return &data[active_ - 1];
}


template<typename T>
void DataManager<T>::deactivate(unsigned index) {
	dirty_flag = true;
	assert(index < active_);
	{
		using std::swap;
		swap(data[index], data[active_ - 1]);
	}
	data[index].setIndex(index);
	data[active_ - 1].setIndex(active_ - 1);
	data[index].updatePointers(data[active_ - 1]);
	--active_;
}
