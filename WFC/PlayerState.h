#pragma once
#include "Command.h"
#include "UnitComponent.h"
enum class sel_state { DEFAULT, MOVING };
struct PlayerState {
	Command* command;
	UnitComponent* selected;
	int team;
	sel_state state;
	PlayerState();
	void switchCommand(Command* new_command);
	void deleteCommand();
};