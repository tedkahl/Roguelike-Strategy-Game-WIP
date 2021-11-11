#pragma once
#include <algorithm>
#include <array>
#include <optional>
#include <concepts>
#include "Pattern.h"
#include "Board.h"
#include "Data.h"
#include "UnitComponent.h"
#include "Team.h"
#include "Attacks.h"
struct Board;

const std::array<sf::Vector2i, 4> dir = { sf::Vector2i(0,-1),sf::Vector2i(1,0),sf::Vector2i(0,1),sf::Vector2i(-1,0) };


static enmity_t getEnmity(Square& s, hasTeam auto u2)
{
	return (!u2 || !s.unit_uc()) ? enmity_t::ENMITY_NULL : getEnmity(u2->team(), s.unit_uc());
}

static void bind_min_max(int& minX, int& maxX, int& minY, int& maxY, sf::Vector2i new_pos) {
	if (new_pos.x < minX)
		minX = new_pos.x;
	else if (new_pos.x > maxX)
		maxX = new_pos.x;
	if (new_pos.y < minY)
		minY = new_pos.y;
	else if (new_pos.y > maxY)
		maxY = new_pos.y;
}

//unused currently
template<typename T>
std::vector<T> get_adj(matrix<T>& grid, sf::Vector2i loc) {
	std::vector<T> ret;
	for (auto i : dir) {
		if (on_board(grid, loc + i))
			ret.push_back(grid.at(loc + i));
	}
	return ret;
}


//static bool block_attack(attack_type type, sf::Vector2i start, sf::Vector2i end, Board& state) {
//	return false;
//}

static int getMovesRem(UnitComponent* u) {
	if (u->getMoveState() == unit::move_state::HAS_MOVED) return 0;
	else return u->stats().movement;
}


static int getMoveCost(UnitComponent* u, /*sf::Vector2i& start,*/ sf::Vector2i end, Board& state) {
	if (isNE(getEnmity(u, state.board.at(end).unit_uc()))) return 999;
	//std::cout << "getting cost: movetype " << u->stats().movetype << " terrain type " << state.board.at(end).type() << std::endl;
	return Data<char>::d()->getMoveCost(u->stats().movetype, state.board.at(end).type());
}

//if team == -1 don't check unit blocking
static int getMoveCost(move_type type, int team, /*sf::Vector2i& start,*/ sf::Vector2i end, Board& state) {
	if (team != -1 && isNE(getEnmity(team, state.board.at(end).unit_uc()))) return 999;
	//std::cout << "getting cost: movetype " << u->stats().movetype << " terrain type " << state.board.at(end).type() << std::endl;
	return Data<char>::d()->getMoveCost(type, state.board.at(end).type());
}