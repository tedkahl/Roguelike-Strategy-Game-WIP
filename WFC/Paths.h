#pragma once
#include "PathsUtil.h"
#include "Attacks.h"
#include "vec2i_util.h"


struct pathsGrid {
	matrix<map_node> grid;
	sf::Vector2i offset; //position of upper left corner on full grid
	unsigned search;
	std::optional<std::vector<sf::Vector2i>> getPath(sf::Vector2i& dest);
	//std::optional<Square> getSquare(sf::Vector2i& loc) { if (on_board(loc - offset, grid)) return grid.at(loc - offset); else return std::nullopt; };
	bool is_movable(sf::Vector2i loc) { return on_board(loc - offset, grid) && grid.at(loc - offset).search == search && grid.at(loc - offset).movable; }
	bool is_attackable(sf::Vector2i loc) { return  on_board(loc - offset, grid) && grid.at(loc - offset).search == search && grid.at(loc - offset).attackable; }
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

static matrix<dj_map_node>& djikstraMap(move_type type, int team, std::vector<UnitComponent*> enemy_targets, Board& state, AttackFxn<dj_map_node> attackfxn) {
	static matrix<dj_map_node> grid;
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

	std::vector<dist_loc> to_process;
	//mark a spot as able to attack the current enemy target, and push it to the heap. If a spot can reach multiple targets mark all of them.
	auto mark_and_add = [&](sf::Vector2i pos) {
		if (grid.at(pos).search != search_counter) {
			grid.at(pos) = dj_map_node(0, curr_pos, { curr_pos }, search_counter);
			to_process.push_back(dist_loc(0, pos));
		}
		else {
			grid.at(pos).reachable_targets.push_back(curr_pos);
		}
	};

	for (unsigned i = 0;i < enemy_targets.size();i++) {
		curr_pos = enemy_targets[i]->getOwner()->getPos();
		grid.at(curr_pos) = dj_map_node(0, sf::Vector2i(-1, -1), {}, search_counter);
		to_process.push_back(dist_loc(0, curr_pos));
		attackfxn(grid, curr_pos, state, mark_and_add);
	}

	int counter = 0;
	while (!to_process.empty()) {
		curr_pos = to_process[0].loc;
		dj_map_node& curr_node = grid.at(curr_pos);
		std::pop_heap(to_process.begin(), to_process.end());
		to_process.pop_back();
		//for each direction

		for (auto& i : dir) {
			counter++;
			if (curr_pos + i != curr_node.prev && on_board(curr_pos + i, state.board)) {
				sf::Vector2i adj_pos = curr_pos + i;
				int new_dist = curr_node.moves_left + getMoveCost(type, team, adj_pos, state);
				if (new_dist > 75) continue;
				//if adjacent square is reachable as easily from another path, don't look further
				if (grid.at(adj_pos).search == search_counter && (grid.at(adj_pos).moves_left == 0 || grid.at(adj_pos).moves_left <= new_dist)) continue;
				//cout << grid.at(adj_pos).search << " " << search_counter << endl;
				//cout << "update " << to_string(adj_pos) << " " << grid.at(adj_pos).moves_left << " to " << new_dist << endl;

				grid.at(adj_pos) = dj_map_node(new_dist, curr_pos, {}, search_counter);
				to_process.push_back(dist_loc(new_dist, adj_pos));
				std::push_heap(to_process.begin(), to_process.end());
			}
		}
	}
	std::cout << counter << std::endl;
	return grid;
}

static void bind_min_max(int& minX, int& maxX, int& minY, int& maxY, sf::Vector2i new_pos) {
	if (new_pos.x < minX)
		minX = new_pos.x;
	else if (new_pos.x > maxX)
		maxX = new_pos.x;
	if (new_pos.y < minY)
		minY = new_pos.y;
	else if (new_pos.y > maxY)
		maxY = new_pos.y;
}
/*What to do: a modified djikstras. We maintain a second board array of "map locations", with the current cost to that square and the previous node of the shortest path. BFS through these
*/
static pathsGrid pathFind(UnitComponent* u, Board& state, AttackFxn<map_node> attackfxn) {
	std::cout << "calling pathFind" << std::endl;
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
	map_node& orig_pos = grid.at(curr_pos);
	grid.at(curr_pos) = map_node(getMovesRem(u), sf::Vector2i(-1, -1), search_counter, true, false);

	auto mark_attackable = [&](sf::Vector2i pos) {
		grid.at(pos).attackable = true;
		bind_min_max(minX, maxX, minY, maxY, pos);
		if (grid.at(pos).search != search_counter || (!grid.at(pos).movable && (grid.at(pos).moves_left < grid.at(curr_pos).moves_left))) {
			//cout << "In attack function: replacing " << to_string(pos) << " with: moves_l " << grid.at(curr_pos).moves_left << " prev " << to_string(curr_pos) << endl;
			grid.at(pos) = map_node(grid.at(curr_pos).moves_left, curr_pos, search_counter, false, true);
			return;
		}
		grid.at(pos).search = search_counter;
	};

	//works as a heap so that duplicate locations will be processed at the same time, cutting off processing of the slower path 
	std::vector<sf::Vector2i> to_process(1, curr_pos);
	while (!to_process.empty()) {
		curr_pos = to_process[0];
		map_node& curr_node = grid.at(curr_pos);
		std::pop_heap(to_process.begin(), to_process.end(), compV2i);
		to_process.pop_back();

		//for each direction from our current location
		for (auto& i : dir) {
			//if the square is where we came from or not on the board, continue
			if (curr_pos + i == curr_node.prev || !on_board(curr_pos + i, state.board)) continue;

			sf::Vector2i adj_pos = curr_pos + i;
			int new_moves_left = curr_node.moves_left - getMoveCost(u, adj_pos, state);
			//if the square is unreachable, continue
			if (new_moves_left < 0) continue;
			map_node& adj_node = grid.at(adj_pos);

			//if we've moved to this node before check if we had a better path before, and continue if so
			if (adj_node.search == search_counter && adj_node.movable) {
				if (grid.at(adj_pos).moves_left >= new_moves_left) continue;
			}
			else {
				//if we haven't moved to this node before, mark if it contains an allied unit
				adj_node.has_ally = (state.board.at(adj_pos).unit() && state.board.at(adj_pos).unit_uc() != u);
				//then if not, get attack range from that square
				if (!adj_node.has_ally)
					attackfxn(grid, adj_pos, state, mark_attackable);
			}
			//then set remaining values, update min and max range, and add the new node to the heap
			adj_node.moves_left = new_moves_left;
			adj_node.prev = curr_pos;
			adj_node.search = search_counter;
			adj_node.movable = true;
			assert(!(grid.at(adj_pos).search == search_counter && grid.at(adj_pos).prev.x == -1));
			bind_min_max(minX, maxX, minY, maxY, adj_pos);
			to_process.push_back(adj_pos);
			std::push_heap(to_process.begin(), to_process.end(), compV2i);
			//cout << "Replacing " << to_string(adj_pos) << " with: moves_l " << new_moves_left << " prev " << to_string(curr_pos) << endl;
		}
	}
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