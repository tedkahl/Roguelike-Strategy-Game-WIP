#pragma once
#include "Command.h"
#include "UnitComponent.h"
#include "Player.h"
enum class sel_state { DEFAULT, MOVING };
struct PlayerState : public Player {
	Command* command;
	Entity* selected;

	PlayerState(int team);
	void deSelect();
	void switchCommand(Command* new_command);
	void deleteCommand();
	void startTurn(DataManager<UnitComponent> units);
};