#pragma once
#include "UnitComponent.h"
#include "Team.h"
#include "PlayerState.h"
#include "Paths.h"
#include "Level.h"
#include "Debug.h"
#include "Actions.h"
struct PlayerState;
class Targeter {
private:
	Level* level;
	unsigned batch;
public:
	Targeter(pathsGrid& paths, Level* level);
	~Targeter();
};

class Command {
protected:
	Level* level;

	Entity* agent_;
	std::unique_ptr<Targeter> targeter;
public:
	Command(Entity* agent, Level& level_);
	virtual void showTargeter() = 0;
	virtual void hideTargeter() = 0;
	virtual std::optional<unit::move_state> execute(sf::Vector2i target, sf::Time now, PlayerState* p_state) = 0; //target can be changed to its own class if necessary
	virtual pathsGrid& getPaths() = 0;
};

class AttackMove :public Command {
private:
	pathsGrid paths;
public:
	AttackMove(Entity* agent, Level& level_);
	virtual void showTargeter() override;
	virtual void hideTargeter() override;
	virtual std::optional<unit::move_state> execute(sf::Vector2i target, sf::Time now, PlayerState* p_state) override;
	virtual pathsGrid& getPaths();
};
