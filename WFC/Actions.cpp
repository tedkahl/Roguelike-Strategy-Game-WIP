#pragma once
#include "Actions.h"
#include "Entity.h"
#include <memory>
#include "UnitComponent.h"
#include "Debug.h"

void actions::attack(Level& level, sf::Vector2i target, Entity* e) {
	UnitComponent* me = e->uc();
	UnitComponent* enemy = level.state.board.at(target).unit_uc();
	assert(me && enemy);
	assert(on_board(target, level.state.board));
	bool dead = enemy->damage(me->stats().damage);
	if (dead) level.killUnit(me, enemy->getOwner());
}