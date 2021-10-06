#include "Player.h"
void Player::setMoveState(UnitComponent* u, std::optional<UnitComponent::move_state> new_state) {
	if (!new_state) return;
	if (u->movestate != UnitComponent::move_state::HAS_ACTED && new_state == UnitComponent::move_state::HAS_ACTED)
		num_moved++;
	else if (u->movestate == UnitComponent::move_state::HAS_ACTED && new_state != UnitComponent::move_state::HAS_ACTED)
		num_moved--;
	u->movestate = new_state.value();
}

//make this dumb for now why bother bookkeeping
//or not
UnitComponent* Player::getNext() {
	if (num_moved == my_team.size())return nullptr;
	return my_team[num_moved];
}

void Player::getUnits(DataManager<UnitComponent>& units) {
	int en;
	num_moved = 0;
	for (auto& i : units) {
		en = Alliance::instance()->getEnmity(team_, &i);
		if (en == enmity::ENEMY) {
			enemies.push_back(&i);
		}
		else if (en == enmity::SAME_TEAM) {
			i.movestate = UnitComponent::move_state::FREE;
			my_team.push_back(&i);
		}
	}
}