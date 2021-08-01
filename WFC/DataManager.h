#pragma once
#include <array>
#include <string>
#include "ResourceManager.h"
#include "assert.h"
#include "DrawComponent.h"
template<typename T>
class DataManager {
private:
	static const size_t MAX = 2000;
	std::array<T, MAX> data;
	unsigned active = 0;
public:
	T* firstInvalidPtr() { return &data[active]; }
	T* get(unsigned index) { return &data[index]; }
	//unsigned activate();

	template<typename ...T_>
	T* declareNew(T_... args);

	auto deactivate(unsigned index);
	typedef std::array<T, MAX>::iterator iterator;
	iterator begin() { return data.begin(); }
	iterator end() { return data.begin() + active; }

};
//Duelist 6,-33
//template<typename T>
//unsigned DataManager<T>::activate()
//{
//	active++;
//	assert(active <= MAX);
//	return active - 1;
//}

template<typename T>
template<typename ...T_>
T* DataManager<T>::declareNew(T_... args)
{
	active++;
	assert(active <= MAX);
	data[active - 1].set(args..., active - 1);
	return &data[active - 1];
}

//return owner pointer (for components) or coords (for entities) for updating
template<typename T>
auto DataManager<T>::deactivate(unsigned index) {
	std::swap(data[index], data[active - 1]);
	data[index].setIndex(index);
	auto owner = data[index].getOwner();
	T* newptr = &data[index];
	--active;
	return std::make_pair(owner, newptr);
}
