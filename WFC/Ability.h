#pragma once
#include "Command.h"
#include "vec2i_util.h"
//#include <ranges>
class Ability
{
};

class Kick : public Command {
private:
	const int dist = 5;
	pathsGrid targets;
public:
	Kick(Entity* agent, Level&);
	virtual void showTargeter() override;
	virtual pathsGrid& getPaths() override;
	virtual std::optional<unit::move_state> execute(sf::Vector2i target, sf::Time now, Player* p_state) override;
};
