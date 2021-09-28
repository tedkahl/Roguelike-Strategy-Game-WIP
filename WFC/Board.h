#pragma once
#include "Square.h"
//#include "DataManager.h"
#include "matrix.h"
class Square;
class Entity;
struct Board {
	matrix<Square> board;
	matrix<float> heightmap;
	void moveEntity(Entity* e, sf::Vector2i newpos);
};