#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <algorithm>
#include "assert.h"

template<typename T>
concept SFResource = requires(T t) {
	t.loadFromFile("");
};

template<typename T>
requires SFResource<T>
class ResourceManager {
private:
	std::map<std::string, size_t> resources;
	std::array<T, 100> resources_;
	int index = 0;
public:
	ResourceManager() {};
	T& get(std::string name);
	T& make(std::string name, T&& resource);
};

template<typename T>
requires SFResource<T>
T& ResourceManager<T>::get(std::string name) {
	auto search = resources.find(name);
	if (search == resources.end()) {
		index++;
		if (!resources_[index].loadFromFile(name)) std::cerr << "error loading " << name << std::endl;

		auto inserted = resources.emplace(std::make_pair(name, index));
		assert(inserted.second);
		return resources_.data()[index];
	}
	return resources_.data()[((*search).second)];
}

template<typename T>
requires SFResource<T>
T& ResourceManager<T>::make(std::string name, T&& resource) {
	auto search = resources.find(name);
	if (search == resources.end()) {
		assert(!resources.contains(name));

		auto inserted = resources.emplace(std::make_pair(name, std::move(resource)));
		assert(inserted.second);
		return *inserted.first->second;
	}
	return *search->second;
}

