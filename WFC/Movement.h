#pragma once
#include <SFML/Graphics.hpp>
#include "BoardState.h"
#include "DrawComponent.h"

class Movement
{
protected:
	sf::Time start_time;
public:
	inline Movement(sf::Time start_t) :start_time(start_t) {};
	virtual bool isFinished(sf::Time current) = 0;
	virtual sf::Vector2f getSpritePos(sf::Time current) = 0;
	virtual sf::Vector2i getCoordChange(sf::Time current) { return sf::Vector2i(0, 0); }
};

class GridMove : public Movement {
private:
	BoardState& state;
	std::vector<sf::Vector2i> path_;
	sf::Time speed_;
public:
	GridMove(std::vector<sf::Vector2i>&& path, sf::Time speed, sf::Time start_t, BoardState& b_state);
	virtual bool isFinished(sf::Time current) override;
	virtual sf::Vector2f getSpritePos(sf::Time current) override;
	virtual sf::Vector2i getCoordChange(sf::Time current) override;
};
