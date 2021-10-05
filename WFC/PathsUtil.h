#pragma once
#include <algorithm>
#include <array>
#include <optional>
#include <concepts>
#include "Pattern.h"
#include "Board.h"
#include "Data.h"
#include "UnitComponent.h"
template <typename T>
concept hasTeam = requires(T t)
{
	{t->team()} -> std::same_as<int>;
};
template <typename T1, typename T2>
static constexpr bool isEnemy(T1 u1, T2 u2)
{
	int a, b;
	if constexpr (hasTeam<T1>)
		a = u1->team();
	else {
		static_assert(std::is_same_v<T1, int>);
		a = u1;
	}
	if constexpr (hasTeam<T2>)
		b = u2->team();
	else {
		static_assert(std::is_same_v<T2, int>);
		b = u2;
	}
	return a == b;
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

static bool block_attack(int type, sf::Vector2i start, sf::Vector2i end, Board& state) {
	return false;
}

static int getMoveCost(UnitComponent* u, int moves_left, sf::Vector2i& start, sf::Vector2i end, Board& state) {
	if (isEnemy(u, state.board.at(end).unit_uc())) return 999;
	//std::cout << "getting cost: movetype " << u->stats().movetype << " terrain type " << state.board.at(end).type() << std::endl;
	return Data<char>::d()->movecosts[u->stats().movetype][state.board.at(end).type()];
}

static int getMoveCost(move_type type, int team, int moves_left, sf::Vector2i& start, sf::Vector2i end, Board& state) {
	if (isEnemy(team, state.board.at(end).unit_uc())) return 999;
	//std::cout << "getting cost: movetype " << u->stats().movetype << " terrain type " << state.board.at(end).type() << std::endl;
	return Data<char>::d()->movecosts[type][state.board.at(end).type()];
}