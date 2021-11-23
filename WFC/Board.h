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

static sf::Vector2f gridPosFromScreenCoords(sf::Vector2f coords, size_t board_h)
{
	float x_pos = coords.y + (.5 * coords.x);
	float y_pos = coords.y - (.5 * coords.x);
	return { x_pos / sq::square_h - static_cast<float>(board_h) / 2,  y_pos / sq::square_h + static_cast<float>(board_h) / 2 };
}