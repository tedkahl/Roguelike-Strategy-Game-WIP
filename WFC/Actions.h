#pragma once
#include <SFML/Graphics.hpp>
#include "Level.h"
class Entity;
class Level;
namespace actions {
	static void attack(Level& level, sf::Vector2i target, Entity* e);
}