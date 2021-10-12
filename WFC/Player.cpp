#include "Player.h"
#include <iostream>
void Player::setMoveState(UnitComponent* u, std::optional<unit::move_state> new_state) {
	if (!new_state) return;
	if (u->movestate != unit::move_state::HAS_ACTED && new_state.value() == unit::move_state::HAS_ACTED)
		num_moved++;
	else if (u->movestate == unit::move_state::HAS_ACTED && new_state.value() != unit::move_state::HAS_ACTED)
		num_moved--;
	u->movestate = new_state.value();
}

//make this dumb for now why bother bookkeeping
//or not
UnitComponent* Player::getNext(DataManager<UnitComponent>& units) {
	if (units.isChanged())
		getUnits(units);
	if (num_moved >= my_team.size()) {
		std::cout << "num moved = team size, turn over" << std::endl;
		return nullptr;
	}
	return my_team[num_moved];
}

void Player::p_start_turn(DataManager<UnitComponent>& units) {
	num_moved = 0;
	units.setChanged(true);
	getUnits(units);
	//warning, may not always hold
	for (auto i : my_team) {
		i->movestate = unit::move_state::FREE;
	}
}

void Player::getUnits(DataManager<UnitComponent>& units) {
	assert(units.isChanged());
	my_team.clear();
	enemies.clear();
	for (auto& i : units) {
		auto en = getEnmity(team_, &i);
		if (en == enmity_t::ENEMY) {
			enemies.push_back(&i);
		}
		else if (en == enmity_t::SAME_TEAM) {
			my_team.push_back(&i);
		}
	}
	units.setChanged(false);
}