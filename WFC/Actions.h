#pragma once
#include <SFML/Graphics.hpp>
#include "Level.h"
class Entity;
class Level;
namespace actions {
	void attack(Level& level, sf::Vector2i target, Entity* e, int damage);
	void heal(Level& level, sf::Vector2i target, Entity* e, int amount);
}