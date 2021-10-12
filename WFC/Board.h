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

template<typename T>
static bool on_board(sf::Vector2i loc, matrix<T>& state) {
	return loc.x >= 0 && loc.x < static_cast<int>(state.width()) && loc.y >= 0 && loc.y < static_cast<int>(state.height());
}