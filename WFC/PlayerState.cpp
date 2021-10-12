#include "PlayerState.h"

PlayerState::PlayerState(int team) :Player(team), selected(nullptr) {}
void PlayerState::switchCommand(Command* new_command) {
	command.reset(new_command);
}

void PlayerState::startTurn(DataManager<UnitComponent>& units) {
	p_start_turn(units);
}

void PlayerState::deleteCommand() {
	command = nullptr;
}

void PlayerState::deSelect() {
	selected = nullptr;
}

void PlayerState::unit_wait(UnitComponent* u) {
	setMoveState(selected->uc(), unit::move_state::HAS_ACTED);
	deSelect();
	deleteCommand();
}