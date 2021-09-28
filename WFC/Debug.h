#pragma once
#include <string>
#include <SFML/Graphics.hpp>
static std::string to_string(sf::Vector2i vec) {
	return std::to_string(vec.x) + ", " + std::to_string(vec.y);
}