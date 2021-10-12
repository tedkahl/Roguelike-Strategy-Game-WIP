#pragma once
#include <vector>
#include <optional>
#include "UnitComponent.h"
#include "Team.h"
#include "DataManager.h"
class Player {
protected:
	int team_;
	int num_moved;
	std::vector<UnitComponent*> my_team;
	std::vector<UnitComponent*> enemies;
	void p_start_turn(DataManager<UnitComponent>& units);
public:
	Player(int team) : team_(team), my_team(), enemies(), num_moved(0) {}
	void getUnits(DataManager<UnitComponent>& units);
	int team() { return team_; }
	UnitComponent* getNext(DataManager<UnitComponent>& units);
	void setMoveState(UnitComponent* u, std::optional<unit::move_state> new_state);
	void undoLastMove() {
		num_moved--;
		assert(num_moved > 0);
		assert(my_team[num_moved]->movestate == unit::move_state::FREE);
	}
};

