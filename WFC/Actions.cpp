#pragma once
#include "Actions.h"
#include "Entity.h"
#include <memory>
#include "UnitComponent.h"
#include "Debug.h"

void actions::attack(Level& level, sf::Vector2i target, Entity* e, int amount) {
	UnitComponent* me = e->uc();
	UnitComponent* enemy = level.state.board.at(target).unit_uc();
	cout << "Attacking!" << endl;
	assert(me && enemy);
	assert(on_board(target, level.state.board));
	bool dead = enemy->damage(amount);
	if (dead) level.killUnit(me, enemy->getOwner());
}

void actions::heal(Level& level, sf::Vector2i target, Entity* e, int amount) {
	UnitComponent* me = e->uc();
	UnitComponent* ally = level.state.board.at(target).unit_uc();
	assert(me && ally);
	assert(on_board(target, level.state.board));
	ally->damage(-amount);
}