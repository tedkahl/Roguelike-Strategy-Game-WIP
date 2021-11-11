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
	std::map<std::string, std::unique_ptr<T>> resources;
	//std::vector<unique_ptr<T>> resources_;
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
		std::unique_ptr<T> resource(new T());
		if (!resource->loadFromFile(name)) std::cerr << "error loading " << name << std::endl;

		auto inserted = resources.emplace(std::make_pair(name, std::move(resource)));
		assert(inserted.second);
		return *inserted.first->second;
	}
	return *search->second;
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

