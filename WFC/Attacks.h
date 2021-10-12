#pragma once
#include <functional>
#include "Board.h"

//a function that determines attack range from a specific location, and calls a given function for each targetable square.
template<typename NodeType>
using AttackFxn = std::function<void(matrix<NodeType>&, sf::Vector2i, Board&, const std::function<void(sf::Vector2i)>&)>;

template<typename NodeType>
static void melee(matrix<NodeType>& grid, sf::Vector2i pos, Board& board, const std::function<void(sf::Vector2i)>& protocol) {
	for (auto& i : dir) {
		if (on_board(pos + i, board.board)) {
			protocol(pos + i);
		}
	}
}

template<typename NodeType>
AttackFxn<NodeType> getAttack(attack_type type) {
	switch (type) {
	case attack_type::MELEE:
		return melee<NodeType>;
		break;

	}
	assert(false);//always find
}