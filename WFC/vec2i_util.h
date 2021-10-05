#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"
static int sumSq(sf::Vector2i val) {
	return pow(val.x, 2) + pow(val.y, 2);
}
static int distSq(const Entity* a, const Entity* b) {
	return sumSq(a->getPos() - b->getPos());
}