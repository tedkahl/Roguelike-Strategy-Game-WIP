#pragma once
#include <algorithm>
#include <array>
#include <optional>
#include "Pattern.h"
#include "Board.h"
#include "Data.h"
#include "UnitComponent.h"
const std::array<sf::Vector2i, 4> dir = { sf::Vector2i(0,-1),sf::Vector2i(1,0),sf::Vector2i(0,1),sf::Vector2i(-1,0) };

struct Board;
template<typename T>
static bool on_board(sf::Vector2i loc, matrix<T>& state) {
	return loc.x >= 0 && loc.x < static_cast<int>(state.width()) && loc.y >= 0 && loc.y < static_cast<int>(state.height());
}

static bool compV2i(sf::Vector2i& lhs, sf::Vector2i& rhs) {
	return std::tie(lhs.x, lhs.y) < std::tie(rhs.x, rhs.y);
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

//modify this for allied teams?
template<typename T1, typename T2>
requires requires(T1 t1, T2 t2) {
	{t1.team()}->std::same_as<int>;
	{t2.team()}->std::same_as<int>;
}
static bool isEnemy(T1* u1, T2* u2) {
	return u1 && u2 && u1->team() != u2->team();
}
//static bool isEnemy(UnitComponent* u1, UnitComponent* u2) {
//	return u1 && u2 && u1->team() != u2->team();
//}

static bool block_attack(int type, sf::Vector2i start, sf::Vector2i end, Board& state) {
	return false;
}

static int getMoveCost(UnitComponent* u, int moves_left, sf::Vector2i& start, sf::Vector2i end, Board& state) {
	if (isEnemy(u, state.board.at(end).unit_uc())) return 999;
	//std::cout << "getting cost: movetype " << u->stats().movetype << " terrain type " << state.board.at(end).type() << std::endl;
	return Data<char>::d()->movecosts[u->stats().movetype][state.board.at(end).type()];
}

struct pathsGrid {
	matrix<map_node> grid;
	sf::Vector2i offset; //position of upper left corner on full grid
	unsigned search;
	std::optional<std::vector<sf::Vector2i>> getPath(sf::Vector2i& dest);
	//std::optional<Square> getSquare(sf::Vector2i& loc) { if (on_board(loc - offset, grid)) return grid.at(loc - offset); else return std::nullopt; };
	bool is_movable(sf::Vector2i& loc) { return is_attackable(loc) && !grid.at(loc - offset).attack_only && !grid.at(loc - offset).has_ally; }
	bool is_attackable(sf::Vector2i& loc) { return on_board(loc - offset, grid) && grid.at(loc - offset).search == search; }
	pathsGrid(matrix<map_node>& g, unsigned minX, unsigned minY, unsigned maxX, unsigned maxY, unsigned search_) :grid(subMatrix(g, minX, minY, maxX - minX + 1, maxY - minY + 1)), offset(minX, minY), search(search_) {}
	pathsGrid() :grid(), search(0) {}
};


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
/*What to do: a modified djikstras. We maintain a second board array of "map locations", with the current cost to that square and the previous node of the shortest path. BFS through these
*/
static pathsGrid pathFind(UnitComponent* u, Board& state) {
	std::cout << "calling getMoveRange" << std::endl;
	static matrix<map_node> grid;
	static unsigned search_counter = 0;
	auto [minX, minY] = u->getPos();
	int maxX = minX;
	int maxY = minY;
	// Incrementing search_counter means we ignore results from earlier searches.
	++search_counter;
	// Whenever the counter cycles, trash the contents of nodes and restart at 1.
	if (search_counter == 0) {
		grid.resize(0, 0);
		search_counter = 1;
	}

	sf::Vector2i curr_pos = u->getPos();
	int moves_rem = u->stats().movement;
	grid.resize(state.board.height(), state.board.width());
	grid.at(curr_pos) = map_node(moves_rem, sf::Vector2i(-1, -1), search_counter);

	//works as a heap so that duplicate locations will be processed at the same time, cutting off processing of the slower path 
	std::vector<sf::Vector2i> to_process(1, curr_pos);
	int num_edges = 0;
	bool is_edge;
	while (!to_process.empty()) {
		curr_pos = to_process[0];
		map_node& curr_node = grid.at(curr_pos);
		std::pop_heap(to_process.begin(), to_process.end(), compV2i);
		to_process.pop_back();
		//for each direction
		is_edge = false;
		curr_node.has_ally = curr_node.has_ally || (state.board.at(curr_pos).unit() && state.board.at(curr_pos).unit_uc() != u);
		for (auto& i : dir) {
			if (curr_pos + i != curr_node.prev && on_board(curr_pos + i, state.board)) {
				sf::Vector2i adj_pos = curr_pos + i;
				int new_moves_left = curr_node.moves_left - getMoveCost(u, curr_node.moves_left, curr_pos, adj_pos, state);
				//if adjacent square is reachable as easily from another path, don't look further
				if (grid.at(adj_pos).search == search_counter && grid.at(adj_pos).moves_left >= new_moves_left) continue;

				if (new_moves_left >= 0)
				{
					grid.at(adj_pos) = map_node(new_moves_left, curr_pos, search_counter);
					minX = std::min(minX, adj_pos.x);
					minY = std::min(minY, adj_pos.y);
					maxX = std::max(maxX, adj_pos.x);
					maxY = std::max(maxY, adj_pos.y);
					to_process.push_back(adj_pos);
					std::push_heap(to_process.begin(), to_process.end(), compV2i);
				}
				//if an adjacent square is not reachable due to move cost, we consider this square, for now, as an "edge".
				else {
					is_edge = true;
				}
			}
		}
		if (is_edge) num_edges++;
		curr_node.is_edge = curr_node.is_edge && is_edge;
	}
	std::cout << "Edges considered: " << num_edges << std::endl;
	addAttackRange(grid, u, minX, minY, maxX, maxY, search_counter, state);
	return pathsGrid(grid, minX, minY, maxX, maxY, search_counter);
}


//class paths {
//	struct step {
//		sf::Vector2i curr, prev;
//		int moves_left;
//		step(sf::Vector2i curr_, sf::Vector2i prev_, int moves_left_) :curr(curr_), prev(prev_), moves_left(moves_left_) {}
//	};
//
//	std::vector<step> destinations;
//	step find(sf::Vector2i loc);
//	bool contains(sf::Vector2i loc);
//	std::vector<sf::Vector2i> getPath(sf::Vector2i loc);
//};