#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "DCSortable.h"
#include "DrawComponent.h"
#include "assert.h"
#include "type_enums.h"
//#include "Data.h"

class DrawComponent;
class Entity;
class UnitComponent;
class DCSortable;
class Square
{
private:
	terrain_type type_;
	SortedDManager<DrawComponent>* manager;
	DCSortable dc_;
	Entity* unit_;
public:
	sf::Vector2i pos;
	std::vector <Entity*> entities;
	Square() = default;
	DrawComponent* dc() const;
	const DCSortable& dcSortable()const;
	Entity* unit();
	UnitComponent* unit_uc();
	inline terrain_type type() { return type_; }
	bool removeE(Entity* e);
	//placeholder, should be a blocking map in board
	void addE(Entity* e);
	void replaceE(Entity* olde, Entity* newe);
	//std::vector<Entity*>& getEntities() { return entities; }
	Square(terrain_type terrain_t, SortedDManager<DrawComponent>* m, DrawComponent* dcs, sf::Vector2i newpos, matrix<float>& heightmap);

};





