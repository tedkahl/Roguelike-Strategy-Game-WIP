#include "AI.h"
AIPlayer::AIPlayer(Level& level, int team) :PlayerState(team), level_(level), dj_map(nullptr), has_selection(false) {

}
bool AIPlayer::selectNext() {
	auto u = getNext();
	if (u) {
		//CALL DECISION PROCESS HERE
		//REFER TO UNIT SPECIFIC INFO
		selected = u->getOwner();
		command.reset(new AttackMove(selected, level_));
		command->showTargeter();
		has_selection = true;
		return true;
	}
	return false;
}

void AIPlayer::rushClosest(Entity* me, sf::Time now) {
	if (!command->execute(target_from_map(me, *dj_map), now, this)) {
		unit_wait(me->uc());
	}
}

void AIPlayer::executeSelected(sf::Time now) {
	//target probably calculated in first step if we do this correctly
	rushClosest(selected, now);
	setMoveState(selected->uc(), unit::move_state::HAS_ACTED);
	has_selection = false;
}

//initializing dj map this way not really satisfactory but should work for now
void AIPlayer::getMap(move_type type) {
	dj_map = &djikstraMap(type, team_, enemies, level_.state);
	for (unsigned i = 0;i < dj_map->height();i++) {
		for (unsigned j = 0;j < dj_map->width();j++) {
			cout << dj_map->at(j, i).moves_left;
			if (dj_map->at(j, i).moves_left < 10) cout << '_';
			cout << " ";

		}
		cout << endl;
	}
}

void AIPlayer::startTurn() {
	getUnits(level_.units);
	getMap(move_type::WALK);
}
