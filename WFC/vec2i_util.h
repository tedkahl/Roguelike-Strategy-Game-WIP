#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"
static int sumSq(sf::Vector2i val) {
	return static_cast<int>(pow(val.x, 2) + pow(val.y, 2));
}
static int distSq(const Entity* a, const Entity* b) {
	return sumSq(a->getPos() - b->getPos());
}

template<typename T>
static sf::Vector2i getCenter(sf::Rect<T> rect) {
	return sf::Vector2i(static_cast<int>(rect.left + rect.width / 2), static_cast<int>(rect.top + rect.height / 2));
}