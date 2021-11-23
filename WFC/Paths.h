#pragma once
#include "PathsUtil.h"
#include "AIutil.h"
#include "Attacks.h"
#include "vec2i_util.h"
#include "PathingGrids.h"


static sf::Vector2i chooseNewTarget(RangeFxn attack, pathsGrid& paths, sf::Vector2i loc, Board& board) {
	sf::Vector2i target{ -1,-1 };
	auto checkLoc = [&](sf::Vector2i new_loc) {
		if (paths.is_movable(new_loc))
			target = new_loc;
	};
	attack(loc, board, checkLoc);
	return target;
}

struct dist_loc {
	float distance;
	sf::Vector2i loc;
	friend bool operator<(const dist_loc& lhs, const dist_loc& rhs) {
		return std::tie(lhs.distance, lhs.loc.x, lhs.loc.y) < std::tie(rhs.distance, lhs.loc.x, rhs.loc.y);
	}
};

static matrix<dj_map_node>& djikstraMap(move_type type, int team, std::vector<map_target>& targets, Board& state) {
	static matrix<dj_map_node> grid;
	static uint8_t search_counter = 0;

	// Incrementing search_counter means we ignore results from earlier searches.
	++search_counter %= 10;;
	// Whenever the counter cycles, trash the contents of nodes and restart at 1.
	if (search_counter == 0) {
		grid.resize(0, 0);
		search_counter = 1;
	}

	sf::Vector2i curr_pos;
	grid.resize(state.board.height(), state.board.width());

	std::vector<dist_loc> to_process;

	for (unsigned i = 0;i < targets.size();i++) {
		curr_pos = targets[i].loc;
		grid.at(curr_pos) = dj_map_node(0, i, search_counter);
		to_process.push_back(dist_loc(0, curr_pos));
		//attackfxn(grid, curr_pos, state, mark_and_add);
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
			if (on_board(curr_pos + i, state.board)) {
				sf::Vector2i adj_pos = curr_pos + i;
				float new_dist = curr_node.score + getMoveCost(type, team, adj_pos, state) * targets[curr_node.target_].priority;
				if (new_dist > 40) continue;
				//if adjacent square is reachable as easily from another path, don't look further
				if (grid.at(adj_pos).search == search_counter && grid.at(adj_pos).score <= new_dist) continue;
				//cout << grid.at(adj_pos).search << " " << search_counter << endl;
				//cout << "update " << to_string(adj_pos) << " " << grid.at(adj_pos).score << " to " << new_dist << endl;

				grid.at(adj_pos) = dj_map_node(new_dist, curr_node.target_, search_counter);
				to_process.push_back(dist_loc(new_dist, adj_pos));
				std::push_heap(to_process.begin(), to_process.end());
			}
		}
	}
	std::cout << "counter:" << counter << std::endl;
	return grid;
}

/*What to do: a modified djikstras. We maintain a second board array of "map locations", with the current cost to that square and the previous node of the shortest path. BFS through these
*/
static pathsGrid pathFind(UnitComponent* u, Board& state, RangeFxn attackfxn, bool attack_only = false) {
	std::cout << "calling pathFind" << std::endl;
	sf::Clock c;
	static matrix<map_node> grid;
	static uint8_t search_counter = 0;
	auto [minX, minY] = u->getPos();
	int maxX = minX;
	int maxY = minY;
	// Incrementing search_counter means we ignore results from earlier searches.
	++search_counter %= 10;
	// Whenever the counter cycles, trash the contents of nodes and restart at 1.
	if (search_counter == 0) {
		grid.resize(0, 0);
		search_counter = 1;
	}
	sf::Vector2i curr_pos = u->getPos();
	grid.resize(state.board.height(), state.board.width());
	auto moves_rem = attack_only ? 0 : getMovesRem(u);
	grid.at(curr_pos) = map_node(moves_rem, sf::Vector2i(-1, -1), search_counter, true, false);

	sf::Vector2i adj_pos;
	int new_moves_left;
	auto mark_attackable = [&](sf::Vector2i pos) {
		grid.at(pos).attackable = true;
		bind_min_max(minX, maxX, minY, maxY, pos);
		if (grid.at(pos).search != search_counter || (!grid.at(pos).movable && (grid.at(pos).moves_left < grid.at(curr_pos).moves_left))) {
			//cout << "In attack function: replacing " << to_string(pos) << " with: moves_l " << grid.at(curr_pos).moves_left << " prev " << to_string(curr_pos) << endl;
			grid.at(pos) = map_node(new_moves_left, adj_pos, search_counter, false, true);
			return;
		}
		grid.at(pos).search = search_counter;
	};

	attackfxn(curr_pos, state, mark_attackable);

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

			adj_pos = curr_pos + i;
			new_moves_left = curr_node.moves_left - getMoveCost(u, adj_pos, state);
			//if the square is unreachable, continue
			if (new_moves_left < 0) continue;
			map_node& adj_node = grid.at(adj_pos);

			//if we've moved to this node before check if we had a better path before, and continue if so
			if (adj_node.search == search_counter && adj_node.movable) {
				if (grid.at(adj_pos).moves_left >= new_moves_left) continue;
			}
			else {
				//if we haven't moved to this node before, mark if it contains an allied unit
				adj_node.has_ally = (state.board.at(adj_pos).unit() != nullptr);
				//then if not, get attack range from that square
				if (!adj_node.has_ally)
					attackfxn(adj_pos, state, mark_attackable);
			}
			//then set remaining values, update min and max range, and add the new node to the heap
			adj_node.moves_left = new_moves_left;
			adj_node.prev = curr_pos;
			adj_node.search = search_counter;
			adj_node.movable = true;
			assert(!(grid.at(adj_pos).search == search_counter && grid.at(adj_pos).prev.x == -1));
			bind_min_max(minX, maxX, minY, maxY, adj_pos);
			//then push the new square to the heap unless it's at the end of our movement range
			if (new_moves_left == 0) continue;
			to_process.push_back(adj_pos);
			std::push_heap(to_process.begin(), to_process.end(), compV2i);
			//cout << "Replacing " << to_string(adj_pos) << " with: moves_l " << new_moves_left << " prev " << to_string(curr_pos) << endl;
		}
	}
	cout << "time for pathing: " << c.getElapsedTime().asMicroseconds() << endl;
	return pathsGrid(grid, minX, minY, maxX, maxY, search_counter);
}

