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
template<typename T>
static bool on_board(sf::Vector2i loc, matrix<T>& state) {
	return loc.x >= 0 && loc.x < static_cast<int>(state.width()) && loc.y >= 0 && loc.y < static_cast<int>(state.height());
}

struct map_node {
	int moves_left;
	sf::Vector2i prev;
	unsigned search;
	bool is_edge;
	bool has_ally;
	bool attack_only;
	map_node() :moves_left(0), prev(), search(0), attack_only(false), is_edge(true), has_ally(false) {}
	map_node(int moves_l, sf::Vector2i prev_, unsigned search_, bool no_move = false, bool isedge = true) :moves_left(moves_l), prev(prev_), search(search_), attack_only(no_move), is_edge(isedge), has_ally(false) {}
};

static bool block_attack(int type, sf::Vector2i start, sf::Vector2i end, Board& state) {
	return false;
}

static void getAttackRange(matrix<map_node>& grid, unsigned search, int& minX, int& minY, int& maxX, int& maxY, UnitStats& u, sf::Vector2i pos, Board& state) {
	sf::Vector2i new_pos;
	switch (u.attack_type) {
	case attack_type::MELEE:
		for (auto i : dir) {
			new_pos = pos + i;
			if (!on_board(new_pos, state.board)) continue;
			if (grid.at(new_pos).search == search && (!grid.at(new_pos).attack_only || grid.at(new_pos).moves_left >= grid.at(pos).moves_left)) continue;

			if (!block_attack(u.attack_type, pos, new_pos, state)) {
				grid.at(new_pos) = map_node(grid.at(pos).moves_left, pos, search, true, false);
				minX = std::min(new_pos.x, minX);
				minY = std::min(new_pos.y, minY);
				maxX = std::max(new_pos.x, maxX);
				maxY = std::max(new_pos.y, maxY);
			}
		}
	}
}

static void addAttackRange(matrix<map_node>& grid, UnitComponent* u, int& minX, int& minY, int& maxX, int& maxY, unsigned search, Board& state) {
	auto stats = u->stats();
	for (int i = minX;i <= maxX;i++) {
		for (int j = minY;j <= maxY;j++) {
			if (grid.at(i, j).search == search && grid.at(i, j).is_edge && !grid.at(i, j).has_ally) {
				getAttackRange(grid, search, minX, minY, maxX, maxY, stats, sf::Vector2i(i, j), state);
			}
		}
	}
}
static int getMovesRem(UnitComponent* u) {
	if (u->movestate == UnitComponent::move_state::HAS_MOVED) return 0;
	else return u->stats().movement;
}


static int getMoveCost(UnitComponent* u, int moves_left, sf::Vector2i& start, sf::Vector2i end, Board& state) {
	if (Alliance::instance()->getEnmity(u, state.board.at(end).unit_uc()) > enmity::ALLY) return 999;
	//std::cout << "getting cost: movetype " << u->stats().movetype << " terrain type " << state.board.at(end).type() << std::endl;
	return Data<char>::d()->movecosts[u->stats().movetype][state.board.at(end).type()];
}

static int getMoveCost(move_type type, int team, int moves_left, sf::Vector2i& start, sf::Vector2i end, Board& state) {
	if (Alliance::instance()->getEnmity(team, state.board.at(end).unit_uc()) > enmity::ALLY) return 999;
	//std::cout << "getting cost: movetype " << u->stats().movetype << " terrain type " << state.board.at(end).type() << std::endl;
	return Data<char>::d()->movecosts[type][state.board.at(end).type()];
}