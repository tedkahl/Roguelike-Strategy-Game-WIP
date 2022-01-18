#pragma once
#include <SFML/Graphics.hpp>
#include "Level.h"
class Entity;
class Level;
namespace actions {
	void attack(Level& level, UnitComponent* enemy, Entity* e, int damage);
	void attackArea(Level& level, const std::vector<sf::Vector2i> locs, Entity* e, int amount);
	void heal(Level& level, sf::Vector2i target, Entity* e, int amount);
}