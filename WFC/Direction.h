#pragma once
#include <array>
#include <optional>
#include "Pattern.h"
//directions: 0=up, 1=right, 2=down, 3=left
const std::array<size_t, 4> directions = { 0,1,2,3 };
const std::array<int, 4> xdir = { 0,1,0,-1 };
const std::array<int, 4> ydir = { 1,0,-1,0 };

const std::optional<std::pair<int, int>> getCoords(std::pair<int, int> coords, matrix<std::vector<size_t>> &wave, int dir)
{
	coords.first += xdir[dir];
	coords.second += ydir[dir];
	if (coords.first < 0 || coords.first >= wave.size() ||
		coords.second < 0 || coords.second >= wave[0].size()) return std::nullopt;
	return coords;
}