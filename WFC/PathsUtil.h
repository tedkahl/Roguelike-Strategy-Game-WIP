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


const std::array<sf::Vector2i, 4> dir = { sf::Vector2i(0,-1),sf::Vector2i(1,0),sf::Vector2i(0,1),sf::Vector2i(-1,0) };

struct Board;
static enmity_t getEnmity(Square& s, hasTeam auto u2)
{
	return (!u2 || !s.unit_uc()) ? enmity_t::ENMITY_NULL : getEnmity(s.unit_uc(), u2->team());
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

struct map_node {
	int moves_left;
	sf::Vector2i prev;
	bool attackable;
	unsigned search;
	bool has_ally;
	bool movable;
	map_node() :moves_left(0), prev(), search(0), attackable(false), has_ally(false), movable(false) {}
	map_node(int moves_l, sf::Vector2i prev_, unsigned search_, bool movable_, bool attackable_) :moves_left(moves_l), prev(prev_), attackable(attackable_), search(search_), movable(movable_), has_ally(false) {
	}
};

struct dj_map_node {
	int moves_left;
	sf::Vector2i prev;
	std::vector<sf::Vector2i> reachable_targets; //inefficiency of this hopefully justified by relatively few reachable targets
	unsigned search;
	dj_map_node() :moves_left(0), prev({ -1,-1 }), search(0) {}
	dj_map_node(int moves_l, sf::Vector2i prev_, std::vector<sf::Vector2i>&& reachable, unsigned search_) :moves_left(moves_l), prev(prev_), reachable_targets(reachable), search(search_) {}
};

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