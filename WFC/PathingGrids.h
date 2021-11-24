#pragma once
#include "AIutil.h"
struct map_node {
	int moves_left;
	sf::Vector2i prev;
	target_t attackable;
	uint8_t search;
	uint8_t has_ally;
	uint8_t movable;
	map_node() :moves_left(0), prev(), search(0), attackable(target_t::NONE), has_ally(false), movable(false) {}
	map_node(int moves_l, sf::Vector2i prev_, uint8_t search_, uint8_t movable_, target_t attackable_) :moves_left(moves_l), prev(prev_), attackable(attackable_), search(search_), movable(movable_), has_ally(false) {
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

template<typename T>
struct subGrid {
	matrix<T> grid;
	sf::Vector2i offset;
	T& rel_at(sf::Vector2i rel_loc) {
		return grid.at(rel_loc);
	}
	T& abs_at(sf::Vector2i abs_loc) {
		return grid.at(abs_loc - offset);
	}
	const T& rel_at(sf::Vector2i rel_loc) const {
		return grid.at(rel_loc);
	}
	const T& abs_at(sf::Vector2i abs_loc) const {
		return grid.at(abs_loc - offset);
	}
	subGrid(matrix<T>&& s_grid, sf::Vector2i off_s) :grid(std::move(s_grid)), offset(off_s) {}
};

struct pathsGrid : public subGrid<map_node> {
	//matrix<map_node> grid;
	//sf::Vector2i offset; //position of upper left corner on full grid
	unsigned search;
	std::optional<std::vector<sf::Vector2i>> getPath(sf::Vector2i& dest);
	bool is_movable(sf::Vector2i abs_loc) { return on_board(abs_loc - offset, grid) && abs_at(abs_loc).search == search && abs_at(abs_loc).movable && !abs_at(abs_loc).has_ally; }
	bool is_attackable(sf::Vector2i abs_loc) { return  on_board(abs_loc - offset, grid) && abs_at(abs_loc).search == search && abs_at(abs_loc).attackable == target_t::TARGET; }
	bool is_display(sf::Vector2i abs_loc) { return  on_board(abs_loc - offset, grid) && abs_at(abs_loc).search == search && static_cast<int>(abs_at(abs_loc).attackable) >= static_cast<int>(target_t::DISPLAY); }
	pathsGrid(matrix<map_node>& g, int minX, int minY, int maxX, int maxY, int search_) :subGrid(subMatrix(g, minX, minY, maxX - minX + 1, maxY - minY + 1), { minX, minY }), search(search_) {}
	//pathsGrid() :subGrid(), search(0) {}
};

static object_type getTarget(const pathsGrid& grid, sf::Vector2i rel_loc) {
	if (grid.rel_at(rel_loc).search != grid.search) return object_type::NONE;
	else return grid.rel_at(rel_loc).movable ? object_type::MOVESELECT : object_type::ATTACKSELECT;
}
/*
object_type getTarget(const subGrid<target_t>& grid, sf::Vector2i rel_loc) {
	switch (grid.rel_at(rel_loc))
	{
		using enum target_t;
	case NONE: return object_type::NONE;
		break;
	case MOVE_TGT: return object_type::MOVESELECT;
		break;
	default: return object_type::ATTACKSELECT;
	}
}*/