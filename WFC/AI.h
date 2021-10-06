#pragma once
#include "UnitComponent.h"
#include "DataManager.h"
#include "Command.h"
#include <algorithm>
//static void sortByCloseness(Entity* me, std::vector<Entity*>& enemies) {
//	auto entityCmp = [=](const Entity*& e1, const Entity*& e2) {
//		return distSq(me, e1) < distSq(me, e2);
//	};
//	std::sort(enemies.begin(), enemies.end(), entityCmp);
//}


static void rushClosest(Entity* me, Level& level, sf::Time wait_time, std::vector<Entity*>& enemies) {

	//sortByCloseness(me, enemies);

	//Entity* closest = getClosestEnemy(me, enemies);
	auto attack = new AttackMove(me, level);
	attack->showTargeter();
	sf::sleep(wait_time);

}

class AIPlayer {
private:
	//DataManager<UnitComponent>& units;
	std::vector<Entity*> allies;
	std::vector<Entity*> enemies;
	Level& level;
	virtual void getNextMove();
	virtual void takeTurn();
	AIPlayer(Level& level);
};

void AIPlayer::takeTurn() {
	for (auto i : allies) {

	}
}
void AIPlayer::getNextMove() {

}