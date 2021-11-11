#pragma once
#include "UnitComponent.h"
#include <memory>
#include <optional>
class Command;
class Level;
class UnitManager;
class Entity;
class Player {
protected:
	int team_;
	int turn;
	Level& level_;
	bool done_;
	int num_moved = 0;
	unsigned count_movable();
public:
	std::unique_ptr<Command> command;
	Entity* selected;
	Player(Level&, int team);
	inline bool is_done() const { return done_; }
	void deSelect();
	void switchCommand(Command* new_command);
	void deleteCommand();
	void startTurn(UnitManager& units);
	void unit_wait(UnitComponent* u);
	void startTurn();
	void setMoveState(UnitComponent* u, std::optional<unit::move_state> new_state);
	int team() { return team_; }

	UnitComponent* getNext();

	virtual ~Player() {}
};
