#pragma once
#include <string>
#include <SFML/Graphics.hpp>
template<typename T>
static std::string to_string(sf::Vector2<T> vec) {
	return std::to_string(vec.x) + ", " + std::to_string(vec.y);
}