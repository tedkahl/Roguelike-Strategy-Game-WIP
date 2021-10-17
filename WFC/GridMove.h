#pragma once
#include "RealTime.h"
#include "Actions.h"
class GridMove : public RealTime {
private:
	std::vector<sf::Vector2i> path_;
	sf::Time speed_;
	std::optional<sf::Vector2f> getSpritePos(float pos, unsigned index);
	virtual std::optional<sf::Vector2i> getCoords(unsigned index) override;
public:
	GridMove(std::vector<sf::Vector2i>&& path, object_type type, sf::Time speed, Board& board);
	virtual EntityUpdate getUpdate(sf::Time current) override;
};

class CompositeRT : public RealTime {
protected:
	std::optional<sf::Vector2f> getSpritePos(float pos, unsigned index);
	std::deque<RealTime*> queue;
public:
	CompositeRT() = default;
	virtual EntityUpdate getUpdate(sf::Time current) override;
};

class SimpleRT : public RealTime {
protected:
	sf::Time speed_;
	Animation anim;
	entity_action action;
	bool hasTriggered = false;
	SimpleRT(object_type type, anim_state a_state, sf::Time speed, Board& board, entity_action& a = actions::null_action);
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