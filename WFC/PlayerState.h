#pragma once
#include "Command.h"
#include "UnitComponent.h"
enum class sel_state { DEFAULT, MOVING };
struct PlayerState {
	Command* command;
	Entity* selected;
	int team;
	sel_state state;
	PlayerState();
	void deSelect();
	void switchCommand(Command* new_command);
	void deleteCommand();
};