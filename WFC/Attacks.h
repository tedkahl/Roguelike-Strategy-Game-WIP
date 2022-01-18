#pragma once
#include <functional>
#include <algorithm>
#include "Board.h"
#include "vec2i_util.h"
enum class target_t : uint8_t {
	NONE, DISPLAY, TARGET,
};
//type of function passed to RangeFxn
using A_Protocol = std::function<void(sf::Vector2i, target_t)>; // position, is_target
//a function that determines attack range from a specific location, and calls a given function for each targetable square.
using RangeFxn = std::function<void(sf::Vector2i, Board&, const A_Protocol&)>;
struct Board;
const std::array<sf::Vector2i, 4> dir2 = { sf::Vector2i(0,-1),sf::Vector2i(1,0),sf::Vector2i(0,1),sf::Vector2i(-1,0) };
static void melee(sf::Vector2i pos, Board& board, const A_Protocol& protocol) {
	for (auto& i : dir2) {
		if (on_board(pos + i, board.board)) {
			protocol(pos + i, target_t::TARGET);
		}
	}
}
static bool isKickable(UnitComponent* me, Entity* target) {
	if (target == nullptr) return false;
	return (target->type() == object_type::ROCK) || getEnmity(me, target->uc()) == enmity_t::SAME_TEAM;
}

static auto kick_r(int range)
{
	return [=](sf::Vector2i pos, Board& board, const A_Protocol& protocol) {
		for (auto& dir : dir2) {
			if (!on_board(pos + dir, board.board)) continue;
			protocol(pos + dir, target_t::TARGET);

			if (isKickable(board.board.at(pos).unit_uc(), board.board.at(pos + dir).unit())) {
				for (int j = 2;j <= range && on_board(pos + dir * j, board.board);j++) {
					//if height change, stop at previous square
					if (board.heightmap.at(pos + dir * j) != board.heightmap.at(pos + dir * (j - 1))) break;
					protocol(pos + j * dir, target_t::DISPLAY);
					//if blocking unit, stop at current square
					if (board.board.at(pos + j * dir).unit_uc() != nullptr) break;
				}
			}
		}
	};
}
static auto arrow_r(unsigned inner, unsigned outer)
{
	return [=](sf::Vector2i pos, Board& board, const A_Protocol& protocol) {
		size_t width = 0;
		int grow_dir = 1;

		for (int y = std::max(static_cast<int>(pos.y - outer), 0);y < std::min(outer + pos.y + 1, board.board.height());y++)
		{
			for (int x = std::max(static_cast<int>(pos.x - width), 0);x < std::min(width + pos.x + 1, board.board.width());x++)
			{
				if (abs(pos.x - x) + abs(pos.y - y) < inner) continue;
				protocol(sf::Vector2i{ x, y }, target_t::TARGET);
			}
			if (y == pos.y) grow_dir = -1;
			width += grow_dir;
		}
	};
}

//confusion: if we're marking squares as TARGET vs DISPLAY here, why do targets have to be checked later for the presence of an enemy, and only in some cases?
//they shouldn't be I think.
static auto dash_r(int range)
{
	return [=](sf::Vector2i pos, Board& board, const A_Protocol& protocol) {
		for (auto& dir : dir2) {
			for (int i = 0; i < range;i++) {
				if (!on_board(pos + dir * i, board.board)) break;
				//if height change, stop at previous square
				if (board.heightmap.at(pos + dir * i) > board.heightmap.at(pos + dir * (i - 1))) break;
				if (board.board.at(pos + dir * i).unit_uc())
					protocol(pos + dir * i, target_t::TARGET);
				else protocol(pos + dir * i, target_t::DISPLAY);
			}
		}
	};
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
		return arrow_r(3, 5);
		break;
	default:
		assert(false);//always find
		return melee;
	}

}
