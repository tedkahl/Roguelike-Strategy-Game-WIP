#pragma once
#include "UnitComponent.h"
#include "Team.h"
#include "Player.h"
#include "Paths.h"
#include "PathingGrids.h"
#include "Level.h"
#include "GridMove.h"
class Player;

class Targeter {
private:
	Level* level_;
	int batch_;
public:
	template<typename T>
	Targeter(const T& targets, Level* level);
	~Targeter();
};

class Command {
protected:
	Level* level_;

	Entity* agent_;
	std::unique_ptr<Targeter> targeter;
public:
	Command(Entity* agent, Level&);
	virtual void showTargeter() = 0;
	virtual void hideTargeter();
	virtual std::optional<unit::move_state> execute(sf::Vector2i, sf::Time, Player*) = 0; //target can be changed to its own class if necessary
	virtual pathsGrid& getPaths() = 0;
	virtual ~Command() {}
};

class AttackMove :public Command {
private:
	pathsGrid paths;
public:
	AttackMove(Entity* agent, Level&);
	AttackMove(Entity* agent, Level&, pathsGrid&& known_paths);
	virtual void showTargeter() override;
	virtual std::optional<unit::move_state> execute(sf::Vector2i target, sf::Time now, Player* p_state) override;
	virtual pathsGrid& getPaths();
};


//class GenericAnim :public Command {
//private:
//	pathsGrid paths;
//public:
//	GenericAnim(Entity* agent, Level& level_, RangeFxn, action);
//	virtual void showTargeter() override;
//	virtual void hideTargeter() override;
//	virtual std::optional<unit::move_state> execute(sf::Vector2i target, sf::Time now, Player* p_state) override;
//	virtual pathsGrid& getPaths();
//};

