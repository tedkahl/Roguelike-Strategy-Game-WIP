#pragma once
#include <algorithm>
#include "BoardState.h"
#include "Data.h"
#include "Entity.h"
const std::array<size_t, 4> directions = { 0,1,2,3 };
const std::array<sf::Vector2i, 4> dir = { sf::Vector2i(0,-1),sf::Vector2i(1,0),sf::Vector2i(0,1),sf::Vector2i(-1,0) };
const std::array<int, 4> ydir = { -1,0,1,0 };
const std::array<int, 4> xdir = { 0,1,0,-1 };



void Move(BoardEntity& e, std::vector<std::pair<unsigned, unsigned>> path)
{

}

bool on_board(sf::Vector2i loc, BoardState& state) {
	return loc.x >= 0 && loc.x < state.board.width() && loc.y >= 0 && loc.y < state.board.height();
}

//modify this for allied teams?
bool isAlly(UnitComponent* u1, UnitComponent* u2) {
	return u1 && u2 && u1->team() == u2->team();
}

int getMoveCost(UnitComponent* u, sf::Vector2i start, sf::Vector2i end, BoardState& state) {
	if (!isAlly(u, state.board.at(end).unit())) return 999;
	return Data<char>::d().movecosts[u->stats().movetype][state.board.at(end).type()];
}



struct map_node {
	int moves_left;
	sf::Vector2i prev;
	unsigned search;
};

/*What to do: a modified djikstras. We maintain a second board array of "map locations", with the current cost to that square and the previous node of the shortest path. BFS through these

*/
static matrix<map_node>& getMoveRange(UnitComponent* u, BoardState& state) {
	static matrix<map_node> grid;
	static unsigned search_counter = 0;
	auto [minX, minY] = u->getPos();
	unsigned maxX = minX, unsigned maxY = minY;
	// Incrementing search_counter means we ignore results from earlier searches.
	++search_counter;
	// Whenever the counter cycles, trash the contents of nodes and restart at 1.
	if (search_counter == 0) {
		grid.resize(0, 0);
		search_counter = 1;
	}

	sf::Vector2i curr_pos = u->getPos();
	int moves_rem = u->stats().movement;
	grid.at(curr_pos) = map_node(moves_rem, sf::Vector2i(-1, -1), search_counter);

	grid.resize(state.board.height(), state.board.width());
	std::vector<sf::Vector2i> to_process(1, u->getPos());

	while (!to_process.empty()) {
		curr_pos = to_process[0];
		map_node& curr_node = grid.at(curr_pos);

		std::pop_heap(to_process.begin(), to_process.end());

		for (auto i : dir) {
			if (curr_pos + i != curr_node.prev && on_board(curr_pos + i, state)) {
				sf::Vector2i adj_pos = curr_pos + i;
				int cost = getMoveCost(u, curr_pos, adj_pos, state);

				if ((grid.at(adj_pos).search != search_counter || grid.at(adj_pos).moves_left < curr_node.moves_left - cost) && curr_node.moves_left - cost > 0)
				{
					grid.at(adj_pos) = map_node(moves_rem - cost, curr_pos, search_counter);
					to_process.push_back(adj_pos);
					std::push_heap(to_process.begin(), to_process.end());
				}
			}
		}
	}
	return grid;
}