#pragma once
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <iostream>
#include "assert.h"

//Objects which "have" Drawcomponents keep one of these to find them in the sorted data manager. Setter functions update both at the same time
//it would be cute if the values here were stored in a bit vector for maximum efficiency. Maybe later.
class DCSortable {
protected:
	uint8_t zval_;
	uint8_t move_direction; //0 "back", 1 "stationary". An object moving "forward" has 0 move direction and changes its coordinates in advance. 
	uint8_t square_height_; //normalized y offset of corresponding square
	uint8_t x; //just the x coordinate of the square. An arbitrary order to break ties
	uint8_t order_; //0 square, 50 object with height. 25-49 means associated w/ object & drawn before, 51-75 associated w/ object and drawn after, 100+ unassociated & drawn after.
	//sf::Vector2i coords_;
	unsigned batch_;

	inline void setMoveDirection(uint8_t dir, DCSortable& dc_ref) {
		move_direction = dir && (order_>=25);
		dc_ref.move_direction = move_direction;
	}
public:
	DCSortable() :zval_(255), move_direction(0), square_height_(255), order_(255), x(255), batch_(255) {}
	//DCSortable(unsigned zval, int dir, unsigned height, unsigned batch) :zval_(-10), move_direction(-10), height_(-10), batch_(-1) {}
	typedef std::tuple<uint8_t, uint8_t, uint8_t, uint8_t, uint8_t> SortType;
	static const DCSortable& null_value()
	{
		static const DCSortable l;
		return l;
	}
	inline void updateCoords(sf::Vector2i coords, DCSortable& dc_ref) {
		assert(coords.x + coords.y < 255);
		zval_ = static_cast<uint8_t>(coords.x + coords.y);
		x = static_cast<uint8_t>(coords.x);
		dc_ref.zval_ = zval_;
		dc_ref.x = x;
	}
	inline uint8_t zval() const { return zval_; }
	inline uint8_t move_dir() const { return move_direction; }
	//
	inline operator bool() const { return sortVal() != null_value().sortVal(); }
	inline const SortType sortVal() const { return std::tie(zval_, move_direction, square_height_, x, order_); }
	DCSortable& sortable() { return *this; }
	inline int batch() const { return batch_; }
	inline sf::Vector2i coords() const { return { x, zval_ - x }; }
};

