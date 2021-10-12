#pragma once
#include "Command.h"
#include "UnitComponent.h"
#include "Player.h"
class Command;
struct PlayerState : public Player {
	std::unique_ptr<Command> command;
	Entity* selected;

	PlayerState(int team);
	void deSelect();
	void switchCommand(Command* new_command);
	void deleteCommand();
	virtual void startTurn(DataManager<UnitComponent>& units);
	void unit_wait(UnitComponent* u);
	virtual ~PlayerState() {}
};
