#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "DCSortable.h"
#include "DrawComponent.h"
#include "assert.h"
#include "type_enums.h"

class Entity;
class UnitComponent;
class Square
{
private:
	terrain_type type_;
	Entity* unit_;
	sf::Vector2i pos;
public:
	inline sf::Vector2i getPos() const { return pos; }
	std::vector <Entity*> entities;
	Square() = default;
	Entity* unit();
	UnitComponent* unit_uc();
	inline terrain_type type() { return type_; }
	bool removeE(Entity* e);
	void addE(Entity* e);
	void replaceE(Entity* olde, Entity* newe);

	Square(terrain_type terrain_t, DrawComponent* dcs, sf::Vector2i newpos, matrix<float>& heightmap);
};





