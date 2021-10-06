#pragma once
#include "PathsUtil.h"

static bool compV2i(const sf::Vector2i& lhs, const sf::Vector2i& rhs) {
	return std::tie(lhs.x, lhs.y) < std::tie(rhs.x, rhs.y);
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

struct dist_loc {
	int distance;
	sf::Vector2i loc;
	friend bool operator<(const dist_loc& lhs, const dist_loc& rhs) {
		return lhs.distance < rhs.distance || (lhs.distance == rhs.distance && compV2i(lhs.loc, rhs.loc));
	}
};

static matrix<map_node>& djikstraMap(move_type type, int team, std::vector<UnitComponent*> enemy_targets, Board& state) {
	static matrix<map_node> grid;
	static unsigned search_counter = 0;

	// Incrementing search_counter means we ignore results from earlier searches.
	++search_counter;
	// Whenever the counter cycles, trash the contents of nodes and restart at 1.
	if (search_counter == 0) {
		grid.resize(0, 0);
		search_counter = 1;
	}

	sf::Vector2i curr_pos;
	grid.resize(state.board.height(), state.board.width());

	//works as a heap so that duplicate locations will be processed at the same time, cutting off processing of the slower path 
	std::vector<dist_loc> to_process(enemy_targets.size());
	for (int i = 0;i < to_process.size();i++) {
		curr_pos = enemy_targets[i]->getOwner()->getPos();
		to_process[i] = dist_loc(getMovesRem(enemy_targets[i]), curr_pos);
		grid.at(curr_pos) = map_node(getMovesRem(enemy_targets[i]), sf::Vector2i(-1, -1), search_counter);
	}
	int num_edges = 0;
	while (!to_process.empty()) {
		curr_pos = to_process[0].loc;
		map_node& curr_node = grid.at(curr_pos);
		std::pop_heap(to_process.begin(), to_process.end());
		to_process.pop_back();
		//for each direction

		for (auto& i : dir) {
			if (curr_pos + i != curr_node.prev && on_board(curr_pos + i, state.board)) {
				sf::Vector2i adj_pos = curr_pos + i;
				int new_dist = curr_node.moves_left + getMoveCost(type, team, curr_node.moves_left, curr_pos, adj_pos, state);
				//if adjacent square is reachable as easily from another path, don't look further
				if (grid.at(adj_pos).search == search_counter && grid.at(adj_pos).moves_left >= new_dist) continue;

				grid.at(adj_pos) = map_node(new_dist, curr_pos, search_counter);
				to_process.push_back(dist_loc(new_dist, adj_pos));
				std::push_heap(to_process.begin(), to_process.end());
			}
		}
	}

	return grid;
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
	grid.resize(state.board.height(), state.board.width());
	grid.at(curr_pos) = map_node(getMovesRem(u), sf::Vector2i(-1, -1), search_counter);

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