#pragma once
#include <algorithm>
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


sf::Vector2i target_from_map(Entity* me, matrix<map_node>& dj_map) {
	auto pos = me->getPos();
	int start_score = dj_map.at(pos).moves_left;
	auto& next_loc = dj_map.at(dj_map.at(pos).prev);
	while (next_loc.moves_left != 0 && start_score - next_loc.moves_left >= me->uc()->stats().movement) {
		next_loc = dj_map.at(next_loc.prev);
	}
	return next_loc.prev;
}

static void rushClosest(Entity* me, Level& level, sf::Time now, sf::Time wait_time, matrix<map_node>& dj_map) {
	auto attack = std::make_unique<AttackMove>(me, level);
	attack->showTargeter();
	sf::sleep(wait_time);
	attack->execute(target_from_map(me, dj_map), now + wait_time);
}

class AIPlayer {
private:
	//DataManager<UnitComponent>& units;
	Level& level_;
	std::vector<UnitComponent*> my_team;
	std::vector<UnitComponent*> enemies;
	matrix<map_node>& dj_map;
	int team_;
	//virtual void getNextMove();
	void getUnits(DataManager<UnitComponent>& units);
	void getMap(move_type type);
	virtual void takeTurn(sf::Time now);
	AIPlayer(Level& level, int team);
};

void AIPlayer::getUnits(DataManager<UnitComponent>& units) {
	int en;
	for (auto& i : units) {
		en = Alliance::instance()->getEnmity(team_, i);
		if (en == enmity::ENEMY) {
			enemies.push_back(&i);
		}
		else if (en == enmity::SAME_TEAM) {
			my_team.push_back(&i);
		}
	}

}

//initializing dj map this way not really satisfactory but should work for now
AIPlayer::AIPlayer(Level& level, int team) : level_(level), team_(team) {

}

void AIPlayer::getMap(move_type type) {
	dj_map = djikstraMap(type, team_, enemies, level_.state);
}

void AIPlayer::takeTurn(sf::Time now) {
	for (auto i : my_team) {
		rushClosest(i->getOwner(), level_, now, sf::milliseconds(250), dj_map);
	}
}