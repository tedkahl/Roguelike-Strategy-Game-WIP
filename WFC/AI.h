#pragma once
#include <algorithm>
#include "Player.h"
#include "UnitComponent.h"
#include "DataManager.h"
#include "Team.h"
#include "Paths.h"
#include "Command.h"
//static void sortByCloseness(Entity* me, std::vector<Entity*>& enemies) {
//	auto entityCmp = [=](const Entity*& e1, const Entity*& e2) {
//		return distSq(me, e1) < distSq(me, e2);
//	};
//	std::sort(enemies.begin(), enemies.end(), entityCmp);
//}


//this is actually pretty insufficient when there are changes mid-turn
static sf::Vector2i target_from_map(Entity* me, matrix<map_node>& dj_map) {
	auto pos = me->getPos();
	int start_score = dj_map.at(pos).moves_left;
	auto& next_loc = dj_map.at(dj_map.at(pos).prev);
	while (next_loc.moves_left != 0 && start_score - next_loc.moves_left >= me->uc()->stats().movement) {
		next_loc = dj_map.at(next_loc.prev);
	}
	return next_loc.prev;
}

static void rushClosest(Entity* me, Level& level, sf::Time now, Command* cmd, matrix<map_node>& dj_map) {
	cmd->execute(target_from_map(me, dj_map), now);
}

class AIPlayer :public Player {
private:
	std::unique_ptr<Command> currCommand;
	bool has_selection;
	Level& level_;
	matrix<map_node>* dj_map;
	//virtual void getNextMove();
	void getMap(move_type type);
public:
	virtual void startTurn();
	bool hasSelection() { return has_selection; }
	virtual bool selectNext();
	virtual void executeSelected(sf::Time now);
	AIPlayer(Level& level, int team);
};
