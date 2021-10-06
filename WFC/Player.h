#pragma once
#include <vector>
#include "UnitComponent.h"
#include "Team.h"
#include "DataManager.h"
#include "PlayerState.h"
class Player {
protected:
	int team_;
	int num_moved;
	std::vector<UnitComponent*> my_team;
	std::vector<UnitComponent*> enemies;
public:
	Player(int team) : team_(team), my_team(), enemies() {}
	void getUnits(DataManager<UnitComponent>& units);
	int team() { return team_; }
	UnitComponent* getNext();
	void setMoveState(UnitComponent* u, std::optional<UnitComponent::move_state> new_state);
	void undoLastMove() {
		num_moved--;
		assert(num_moved > 0);
		assert(my_team[num_moved]->movestate == UnitComponent::move_state::FREE);
	}
};

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
		en = Alliance::instance()->getEnmity(team_, i);
		if (en == enmity::ENEMY) {
			enemies.push_back(&i);
		}
		else if (en == enmity::SAME_TEAM) {
			i.movestate = UnitComponent::move_state::FREE;
			my_team.push_back(&i);
		}
	}
}