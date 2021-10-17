#pragma once
#include "RealTime.h"


class SimpleRT : public RealTime {
protected:
	sf::Time speed_;
	Animation anim;
	entity_action action;
	bool hasTriggered = false;
	float delay_;
public:
	SimpleRT(object_type type, anim_state a_state, sf::Time speed, Board& board, entity_action& a = actions::null_action, float delay = .0f);
	virtual EntityUpdate getUpdate(sf::Time current) override;
	virtual ~SimpleRT() {}
};

class Lunge : public SimpleRT {
private:
	const float lunge = 15.f;
	sf::Vector2f start_pos;
	sf::Vector2i dir;
	sf::Vector2i target_;
	entity_action action;
	bool hasTriggered = false;

public:
	Lunge(sf::Vector2i pos, sf::Vector2i target, object_type type, anim_state state, sf::Time start_t, Board& board, entity_action& a);
	virtual EntityUpdate getUpdate(sf::Time current) override;
};
