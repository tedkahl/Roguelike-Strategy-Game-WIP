#pragma once
#include "UnitComponent.h"
#include "Paths.h"
#include "Level.h"
#include "Debug.h"
class Targeter {
private:
	Level& level;
	unsigned batch;
public:
	Targeter(pathsGrid& paths, Level& level);
	~Targeter();
};

class Command {
protected:
	Level& level;

	Entity* agent_;
	std::unique_ptr<Targeter> targeter;
public:
	Command(Entity* agent, Level& level_);
	virtual void showTargeter() = 0;
	virtual void hideTargeter() = 0;
	virtual void execute(sf::Vector2i target, sf::Time now) = 0; //target can be changed to its own class if necessary
};

class AttackMove :public Command {
private:
	pathsGrid paths;
public:
	AttackMove(Entity* agent, Level& level_);
	virtual void showTargeter() override;
	virtual void hideTargeter() override;
	virtual void execute(sf::Vector2i target, sf::Time now) override;
};
