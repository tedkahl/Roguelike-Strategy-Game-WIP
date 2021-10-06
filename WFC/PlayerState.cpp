#include "PlayerState.h"

PlayerState::PlayerState(int team) :Player(team), command(nullptr), selected(nullptr) {}
void PlayerState::switchCommand(Command* new_command) {
	if (command) delete command;
	command = new_command;
}

void PlayerState::startTurn(DataManager<UnitComponent> units) {
	getUnits(units);
}

void PlayerState::deleteCommand() {
	delete command;
	command = nullptr;
}

void PlayerState::deSelect() {
	selected = nullptr;
}