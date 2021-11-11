#pragma once
#include <SFML/Graphics.hpp>
//very pointless file right now
struct map_target {
	sf::Vector2i loc;
	float priority = 1.f;
	friend bool operator==(const map_target& l, const map_target& r) { return l.loc == r.loc && l.priority == r.priority; }
};