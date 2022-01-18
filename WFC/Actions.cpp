#pragma once
#include "Actions.h"
#include "Entity.h"
#include <memory>
#include "UnitComponent.h"
#include "Debug.h"

void actions::attack(Level& level, UnitComponent* enemy, Entity* e, int amount) {
	UnitComponent* me = e->uc();
	cout << "Attacking!" << endl;
	assert(me && enemy);
	bool dead = enemy->damage(amount);
	if (dead) level.killUnit(me, enemy->getOwner());
}

void actions::attackArea(Level& level, const std::vector<sf::Vector2i> locs, Entity* e, int amount) {
	UnitComponent* me = e->uc();
	cout << "Attacking!" << endl;
	for (auto& i : locs) {
		if (auto u = level.state.board.at(i).unit_uc()) {
			actions::attack(level, u, e, amount);
		}
	}
}

void actions::heal(Level& level, sf::Vector2i target, Entity* e, int amount) {
	UnitComponent* me = e->uc();
	UnitComponent* ally = level.state.board.at(target).unit_uc();
	assert(me && ally);
	assert(on_board(target, level.state.board));
	ally->damage(-amount);
}