#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "ResourceManager.h"
#include "DCSortable.h"
#include "DrawComponent.h"
#include "assert.h"
//#include "Data.h"

class DrawComponent;
class Entity;
class UnitComponent;
class DCSortable;
class Square
{
private:
	unsigned type_;
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
	inline unsigned type() { return type_; }
	//void setDC(const DCAccessor& dc);
	bool removeE(Entity* e);
	void addE(Entity* e);
	void replaceE(Entity* olde, Entity* newe);
	//std::vector<Entity*>& getEntities() { return entities; }
	Square(unsigned terrain_t, SortedDManager<DrawComponent>* m, DrawComponent* dcs, sf::Vector2i newpos, matrix<float>& heightmap);

};





