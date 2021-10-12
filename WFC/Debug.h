#pragma once
#include <string>
#include <sstream>
#include <SFML/Graphics.hpp>
template<typename T>
static std::string to_string(sf::Vector2<T> vec) {
	return std::to_string(vec.x) + ", " + std::to_string(vec.y);
}
static std::string to_str(sf::IntRect rect) {
	std::stringstream s;
	s << rect.left << " " << rect.top << " " << rect.width << " " << rect.height << std::endl;
	return s.str();
}