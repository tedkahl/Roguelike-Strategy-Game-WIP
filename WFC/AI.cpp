#include "AI.h"
AIPlayer::AIPlayer(Level& level, int team) :Player(team), level_(level) {

}
bool AIPlayer::selectNext() {
	auto u = getNext();
	if (u) {
		//CALL DECISION PROCESS HERE
		//REFER TO UNIT SPECIFIC INFO
		currCommand = std::make_unique<AttackMove>(u->getOwner(), level_);
		currCommand->showTargeter();
		has_selection = true;
		return true;
	}
	return false;
}

void AIPlayer::executeSelected(sf::Time now) {
	//target probably calculated in first step if we do this correctly
	rushClosest(getNext()->getOwner(), level_, now, currCommand.get(), *dj_map);
	has_selection = false;
}
//initializing dj map this way not really satisfactory but should work for now
void AIPlayer::getMap(move_type type) {
	dj_map = &djikstraMap(type, team_, enemies, level_.state);
}

void AIPlayer::startTurn() {
	getMap(move_type::WALK);
	getUnits(level_.units);
}
