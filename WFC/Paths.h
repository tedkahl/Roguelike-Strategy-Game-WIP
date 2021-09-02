#pragma once
#include <algorithm>
#include <array>
#include <optional>
#include "BoardState.h"
#include "Data.h"
#include "UnitComponent.h"
const std::array<sf::Vector2i, 4> dir = { sf::Vector2i(0,-1),sf::Vector2i(1,0),sf::Vector2i(0,1),sf::Vector2i(-1,0) };


static bool on_board(sf::Vector2i loc, BoardState& state) {
	return loc.x >= 0 && loc.x < state.board.width() && loc.y >= 0 && loc.y < state.board.height();
}

static bool compV2i(sf::Vector2i& lhs, sf::Vector2i& rhs) {
	return std::tie(lhs.x, lhs.y) < std::tie(rhs.x, rhs.y);
}


struct map_node {
	int moves_left;
	sf::Vector2i prev;
	unsigned search;
	map_node() :moves_left(0), prev(), search(0) {}
	map_node(int moves_l, const sf::Vector2i& prev_, unsigned search_) :moves_left(moves_l), prev(prev_), search(search_) {}
};

//modify this for allied teams?
static bool isEnemy(UnitComponent* u1, UnitComponent* u2) {
	return u1 && u2 && u1->team() != u2->team();
}

static int getMoveCost(UnitComponent* u, sf::Vector2i start, sf::Vector2i end, BoardState& state) {
	if (isEnemy(u, state.board.at(end).unit())) return 999;
	//std::cout << "getting cost: movetype " << u->stats().movetype << " terrain type " << state.board.at(end).type() << std::endl;
	return Data<char>::d()->movecosts[u->stats().movetype][state.board.at(end).type()];
}

struct pathsGrid {
	matrix<map_node>* grid;
	unsigned minX, minY, maxX, maxY, search;
	std::optional<std::vector<sf::Vector2i>> getPath(sf::Vector2i& dest);
	pathsGrid(matrix<map_node>& g, unsigned mnX, unsigned mnY, unsigned mxX, unsigned mxY, unsigned search_) :grid(&g), minX(mnX), minY(mnY), maxX(mxX), maxY(mxY), search(search_) {}
	pathsGrid() :grid(), minX(0), minY(0), maxX(0), maxY(0), search(0) {}
};
/*What to do: a modified djikstras. We maintain a second board array of "map locations", with the current cost to that square and the previous node of the shortest path. BFS through these
*/

static pathsGrid pathFind(UnitComponent* u, BoardState& state) {
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

	std::vector<sf::Vector2i> to_process(1, curr_pos);

	while (!to_process.empty()) {
		curr_pos = to_process[0];
		map_node& curr_node = grid.at(curr_pos);
		std::cout << "mleft: " << curr_node.moves_left << " prev: " << curr_node.prev.x << ", " << curr_node.prev.y << " search " << curr_node.search << std::endl;
		std::pop_heap(to_process.begin(), to_process.end(), compV2i);
		to_process.pop_back();
		std::cout << "Heap size after popping: " << to_process.size() << std::endl;
		for (auto& i : dir) {
			std::cout << "Checking " << curr_pos.x + i.x << ", " << curr_pos.y + i.y << std::endl;
			if (curr_pos + i != curr_node.prev && on_board(curr_pos + i, state)) {
				sf::Vector2i adj_pos = curr_pos + i;
				int cost = getMoveCost(u, curr_pos, adj_pos, state);

				if ((grid.at(adj_pos).search != search_counter || grid.at(adj_pos).moves_left < curr_node.moves_left - cost) && curr_node.moves_left - cost >= 0)
				{
					grid.at(adj_pos) = map_node(curr_node.moves_left - cost, curr_pos, search_counter);
					minX = std::min(minX, static_cast<int>(adj_pos.x));
					minY = std::min(minY, static_cast<int>(adj_pos.y));
					maxX = std::max(maxX, static_cast<int>(adj_pos.x));
					maxY = std::max(maxY, static_cast<int>(adj_pos.y));
					to_process.push_back(adj_pos);
					std::push_heap(to_process.begin(), to_process.end(), compV2i);
				}
			}
		}
	}

	return pathsGrid(grid, minX, minY, maxX, maxY, search_counter);
}


//class paths {
//	struct step {
//		sf::Vector2i curr, prev;
//		int moves_left;
//		step(const sf::Vector2i& curr_, const sf::Vector2i& prev_, int moves_left_) :curr(curr_), prev(prev_), moves_left(moves_left_) {}
//	};
//
//	std::vector<step> destinations;
//	step find(sf::Vector2i loc);
//	bool contains(sf::Vector2i loc);
//	std::vector<sf::Vector2i> getPath(sf::Vector2i loc);
//};