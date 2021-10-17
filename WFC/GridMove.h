#pragma once
#include "RealTime.h"
#include "RTAction.h"
#include "Actions.h"

class GridMove : public CompositeRT {
private:
	std::vector<sf::Vector2i> path_;
	sf::Time speed_;
	std::optional<sf::Vector2f> getSpritePos(float pos, unsigned index);
public:
	GridMove(std::vector<sf::Vector2i>& path, object_type type, sf::Time speed, Board& board);
	using CompositeRT::getUpdate;
};


class SimpleMove :public SimpleRT {
private:
	sf::Vector2i start_;
	sf::Vector2i end_;
	std::optional<sf::Vector2f> getSpritePos(float fraction);
public:
	SimpleMove(sf::Vector2i start, sf::Vector2i end, object_type type, sf::Time speed, Board& board);
	virtual EntityUpdate getUpdate(sf::Time current) override;
};