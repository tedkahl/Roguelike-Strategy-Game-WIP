#include "Actions.h"

void actions::attack(Level& level, sf::Vector2i target, Entity* e) {
	UnitComponent* me = e->uc();
	UnitComponent* enemy = level.state.board.at(target).unit_uc();
	assert(me && enemy);
	bool dead = enemy->damage(me->stats().damage);
	if (dead) level.killUnit(me, enemy->getOwner());
}