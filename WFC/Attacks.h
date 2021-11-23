#pragma once
#include <functional>
#include <algorithm>
#include "Board.h"
#include "vec2i_util.h"
//a function that determines attack range from a specific location, and calls a given function for each targetable square.
using RangeFxn = std::function<void(sf::Vector2i, Board&, const std::function<void(sf::Vector2i)>&)>;
struct Board;
const std::array<sf::Vector2i, 4> dir2 = { sf::Vector2i(0,-1),sf::Vector2i(1,0),sf::Vector2i(0,1),sf::Vector2i(-1,0) };
static void melee(sf::Vector2i pos, Board& board, const std::function<void(sf::Vector2i)>& protocol) {
	for (auto& i : dir2) {
		if (on_board(pos + i, board.board)) {
			protocol(pos + i);
		}
	}
}
/*
	auto pos = agent_->getPos();
	auto& board = level_->state;
	for (auto& dir : dir2) {
		if (!on_board(pos + dir, board.board)) continue;
		targets.abs_at(pos + dir) = target_t::ATTACK_TGT;

		if (isKickable(board.board.at(pos).unit_uc(), board.board.at(pos + dir).unit())) {
			for (int j = 2;j <= dist;j++) {
				//if height change, stop at previous square
				if (board.heightmap.at(pos + dir * j) != board.heightmap.at(pos + dir * (j - 1))) break;
				targets.abs_at(pos + dir) = target_t::ATTACK_NO_TGT;
				//if blocking unit, stop at current square
				if (board.board.at(pos + j * dir).unit_uc() != nullptr) break;
			}
		}
	}
*/
static bool isKickable(UnitComponent* me, Entity* target) {
	if (target == nullptr) return false;
	return (target->type() == object_type::ROCK) || getEnmity(me, target->uc()) == enmity_t::SAME_TEAM;
}

static auto kick_r(int range, sf::Vector2i pos, Board& board)
{
	return [=](sf::Vector2i pos, Board& board, const std::function<void(sf::Vector2i)>& protocol) {
		for (auto& dir : dir2) {
			if (!on_board(pos + dir, board.board)) continue;
			protocol(pos + dir);

			if (isKickable(board.board.at(pos).unit_uc(), board.board.at(pos + dir).unit())) {
				for (int j = 2;j <= range;j++) {
					//if height change, stop at previous square
					if (board.heightmap.at(pos + dir * j) != board.heightmap.at(pos + dir * (j - 1))) break;
					protocol(pos + j * dir);
					//if blocking unit, stop at current square
					if (board.board.at(pos + j * dir).unit_uc() != nullptr) break;
				}
			}
		}
	};
}
template<unsigned inner, unsigned outer>
static void arrow(sf::Vector2i pos, Board& board, const std::function<void(sf::Vector2i)>& protocol)
{
	size_t width = 0;
	int grow_dir = 1;

	for (int y = std::max(static_cast<int>(pos.y - outer), 0);y <= std::min(outer + pos.y, board.board.height());y++)
	{
		for (int x = std::max(static_cast<int>(pos.x - width), 0);x <= std::min(width + pos.x, board.board.width());x++)
		{
			if (sumSq(pos - sf::Vector2i{ x, y }) < inner * inner) continue;
			protocol(pos + sf::Vector2i{ x, y });
		}
		width += grow_dir;
		if (y == pos.y) grow_dir = -1;
	}
}


//very placeholder
static bool blocking(Square& sq) {
	return sq.unit() && (sq.unit()->type() == object_type::ROCK);
}


//static void beam(sf::Vector2i pos, Board& board, const std::function<void(sf::Vector2i)>& protocol)
//{
//	sf::Vector2i temp_pos;
//	for (auto& i : dir) {
//		temp_pos = pos + i;
//		while (on_board(temp_pos, board.board) {
//			protocol(temp_pos);
//			if (blocking(board.at(temp_pos)) break;
//				temp_pos += i;
//		}
//	}
//}

//we can do better
static RangeFxn getAttack(attack_type type) {
	switch (type) {
	case attack_type::MELEE:
		return melee;
		break;
	case attack_type::RANGED_ARROW:
		return arrow<3, 5>;
		break;
	default:
		assert(false);//always find
		return melee;
	}

}
