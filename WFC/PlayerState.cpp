#include "PlayerState.h"

PlayerState::PlayerState() :command(nullptr), selected(nullptr), team(0), state(sel_state::DEFAULT) {}
void PlayerState::switchCommand(Command* new_command) {
	if (command) delete command;
	command = new_command;
}

void PlayerState::deleteCommand() {
	delete command;
	command = nullptr;
}