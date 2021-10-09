#pragma once
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <iostream>
//#include "DCAccessor.h"
class DCSortable {
protected:
	uint8_t zval_;
	uint8_t move_direction; //0 "back", 1 "stationary". An object
	uint8_t square_height_; //normalized y offset of corresponding square
	uint8_t obj_height_; //0 square, 1 flat object, 2 object with height
	uint8_t order; //just the y coordinate of the square. An arbitrary order to break ties
	//sf::Vector2i coords_;
	unsigned batch_;
public:
	DCSortable() :zval_(-1), move_direction(0), square_height_(-1), obj_height_(-1), order(-1), batch_(-1) {}
	//DCSortable(unsigned zval, int dir, unsigned height, unsigned batch) :zval_(-10), move_direction(-10), height_(-10), batch_(-1) {}
	typedef std::tuple<uint8_t, uint8_t, uint8_t, uint8_t, uint8_t> SortType;
	static const DCSortable& null_value()
	{
		static const DCSortable l;
		return l;
	}
	inline unsigned zval() const { return zval_; }
	inline void updateCoords(sf::Vector2i coords) { zval_ = coords.x + coords.y; order = coords.x; }

	inline void setMoveDirection(uint8_t dir) { move_direction = dir * (obj_height_ >> 1); }
	inline operator bool() const { return sortVal() != null_value().sortVal(); }
	inline const SortType sortVal() const { return std::tie(zval_, move_direction, square_height_, obj_height_, order); }
	DCSortable sortable() const { return *this; }
	inline int batch() const { return batch_; }
};
