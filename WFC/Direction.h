#pragma once
#include <array>
#include <optional>
#include "Pattern.h"
//directions: 0=up, 1=right, 2=down, 3=left
const std::array<size_t, 4> directions = { 0,1,2,3 };
const std::array<int, 4> ydir = { -1,0,1,0 };
const std::array<int, 4> xdir = { 0,1,0,-1 };

const std::optional<std::pair<int, int>> getCoords(int x, int y, const matrix < std::vector<uint8_t>>& wave, int dir)
{
	y += ydir[dir];
	x += xdir[dir];
	if (y < 0 || (unsigned)y >= wave.height() ||
		x < 0 || (unsigned)x >= wave.width()) return std::nullopt;
	return std::make_pair(x, y);
}

const size_t oppositeDir(size_t dir) {
	return (dir + 2) % 4;
}