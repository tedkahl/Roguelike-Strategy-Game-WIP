#pragma once
#include "BoardState.h"
#include "Entity.h"
const std::array<size_t, 4> directions = { 0,1,2,3 };
const std::array<int, 4> ydir = { -1,0,1,0 };
const std::array<int, 4> xdir = { 0,1,0,-1 };

void Move(BoardEntity& e, std::vector<std::pair<unsigned, unsigned>> path)
{

}

void getMoveRange(Unit& u, BoardState& state)
{

}

void getMoveRange_r(Unit& u, int, BoardState& state, int r) {
	for (auto d : directions) {

	}
}