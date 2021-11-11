#pragma once
#include "AIutil.h"
struct map_node {
	int moves_left;
	sf::Vector2i prev;
	bool attackable;
	uint8_t search;
	uint8_t has_ally;
	uint8_t movable;
	map_node() :moves_left(0), prev(), search(0), attackable(false), has_ally(false), movable(false) {}
	map_node(int moves_l, sf::Vector2i prev_, uint8_t search_, uint8_t movable_, uint8_t attackable_) :moves_left(moves_l), prev(prev_), attackable(attackable_), search(search_), movable(movable_), has_ally(false) {
	}
};

struct dj_map_node {
	float score;
	unsigned target_;
	uint8_t search;
	dj_map_node() :score(0u), target_(-1), search(0u) {}
	dj_map_node(float node_score, unsigned target, uint8_t search_) :score(node_score), target_(target), search(search_) {}
};
struct dj_map {
	matrix<dj_map_node> map;
	std::vector<map_target> targets;
};

struct pathsGrid {
	matrix<map_node> grid;
	sf::Vector2i offset; //position of upper left corner on full grid
	unsigned search;
	std::optional<std::vector<sf::Vector2i>> getPath(sf::Vector2i& dest);
	//std::optional<Square> getSquare(sf::Vector2i& loc) { if (on_board(loc - offset, grid)) return grid.at(loc - offset); else return std::nullopt; };
	bool is_movable(sf::Vector2i loc) { return on_board(loc - offset, grid) && grid.at(loc - offset).search == search && grid.at(loc - offset).movable && !grid.at(loc - offset).has_ally; }
	bool is_attackable(sf::Vector2i loc) { return  on_board(loc - offset, grid) && grid.at(loc - offset).search == search && grid.at(loc - offset).attackable; }
	pathsGrid(matrix<map_node>& g, unsigned minX, unsigned minY, unsigned maxX, unsigned maxY, unsigned search_) :grid(subMatrix(g, minX, minY, maxX - minX + 1, maxY - minY + 1)), offset(minX, minY), search(search_) {}
	pathsGrid() :grid(), search(0) {}
};