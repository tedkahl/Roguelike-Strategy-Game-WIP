#pragma once
#include "Command.h"
#include "vec2i_util.h"
//#include <ranges>
class Ability
{
};

class Kick : public Command {
public:
	Kick(Entity* agent, Level&);
	virtual std::optional<unit::move_state> execute(sf::Vector2i target, sf::Time now, Player* p_state) override;
};

class PoisonArrow : public Command {
public:
	PoisonArrow(Entity* agent, Level&);
	virtual std::optional<unit::move_state> execute(sf::Vector2i target, sf::Time now, Player* p_state) override;
};

class Dash : public Command {
public:
	Dash(Entity* agent, Level&);
	virtual std::optional<unit::move_state> execute(sf::Vector2i target, sf::Time now, Player* p_state) override;
};