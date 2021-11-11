#include <ranges>
#include "Player.h"
#include "Command.h"
#include "UnitManager.h"
#include "Level.h"

Player::Player(Level& level, int team) :team_(team), turn(0), level_(level), selected(nullptr) {}
void Player::switchCommand(Command* new_command) {
	command.reset(new_command);
}

void Player::deleteCommand() {
	command = nullptr;
}

void Player::deSelect() {
	selected = nullptr;
}

void Player::unit_wait(UnitComponent* u) {
	setMoveState(selected->uc(), unit::move_state::HAS_ACTED);
	deSelect();
	deleteCommand();
}

void Player::startTurn() {
	turn++;
	done_ = false;
	for (auto& i : (*level_.units) | std::views::filter(unit::allies(team_))) {
		i.movestate = unit::move_state::FREE;
	}
}

//get the next movable unit by searching through the list from scratch. This is fine because it's a small contiguous list and this ensures any changes to the unit list are reflected in selection
UnitComponent* Player::getNext() {
	using namespace std::views;
	//get range of movable allies
	auto movable = (*level_.units) | filter(unit::allies(team_)) | filter(unit::movable());

	//if range is empty return null
	auto start = movable.begin();
	if (start == movable.end()) return nullptr;

	//else get the next unit
	else if (selected) {
		while (&*start <= selected->uc()) start++;

		if (start == movable.end()) {
			return &*movable.begin();
		}
	}
	return &*start;
}
unsigned Player::count_movable() {
	unsigned count = 0;
	using namespace std::views;
	for (auto const& i : (*level_.units) | filter(unit::allies(team_)) | filter(unit::movable())) count++;
	cout << "movable units: " << count << endl;
	return count;
}
void Player::setMoveState(UnitComponent* u, std::optional<unit::move_state> new_state) {
	using namespace std::views;
	if (!new_state) return;
	if (u->movestate != unit::move_state::HAS_ACTED && new_state.value() == unit::move_state::HAS_ACTED)
		num_moved++;
	else if (u->movestate == unit::move_state::HAS_ACTED && new_state.value() != unit::move_state::HAS_ACTED)
		num_moved--;
	u->movestate = new_state.value();
	if (count_movable() == 0) done_ = true;

}