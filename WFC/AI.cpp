#include "AI.h"
#include "Attacks.h"
#include "Team.h"
//#define TEST
//just for movement right now
float getCompositeScore(std::vector<preference>& state, const std::map<preference_type, dj_map>& maps, sf::Vector2i loc) {
	std::partition(state.begin(), state.end(), [](const preference& p) {return !p.additive_;});
	float score = FLT_MAX;
	for (auto& i : state) {
		auto& node = maps.at(i.type_).map.at(loc);
		if (!i.additive_) {
			score = i.multiplier_ * std::min(node.score, score);
		}
		else
			score += i.multiplier_ * node.score;
	}
	return score;
}

//choose move and action based on djikstra maps and pathing grid
void AIPlayer::targetFromMap(Entity* me, pathsGrid& paths) {
	auto pos = me->getPos();
	float min_score = FLT_MAX;

	sf::Vector2i sq;
	std::vector<preference> state{ preference{preference_type::ANY_ENEMY}, preference{preference_type::BREAK_ROCK, .1f, true} };
	target = { -1,-1 };
	for (int y = 0;y < paths.grid.height();y++) {
		for (int x = 0;x < paths.grid.width();x++) {
			if (paths.grid.at(x, y).search == paths.search) {
				sq = sf::Vector2i{ x,y } + paths.offset;
				auto new_score = (getCompositeScore(state, maps, sq));
				if ((paths.grid.at(x, y).movable ||
					paths.grid.at(x, y).attackable == target_t::TARGET &&
					isNE(getEnmity(level_.state.board.at(sf::Vector2i{ x,y } + paths.offset),
						me->uc()))) && new_score < min_score) {
					target = sq;
					min_score = new_score;
				}
			}
		}
	}
	//we don't actually assert this but
	assert(target.x > 0);
	command.reset(new AttackMove(me, level_, std::move(paths)));
}
//get preferences for current unit and retrieve maps if necessary.
/*
* WARNING PLACEHOLDER BUGS HERE
*/
void AIPlayer::getMaps(UnitComponent* unit) {
	//get state from unit->AIComponent.state()
	std::vector<preference> state{ preference{preference_type::ANY_ENEMY}, preference{preference_type::BREAK_ROCK, -.1f, true} };
	for (auto& i : state) {
		auto [ref, inserted] = maps.emplace(std::make_pair(i.type_, dj_map()));
		auto dirty = getTargets(i.type_, team_, *level_.units, *level_.entities, (ref->second).targets);

		//if 
		if (inserted || dirty) {
			(ref->second).map = djikstraMap(move_type::WALK, team_, (ref->second).targets, level_.state);
		}
	}
}
// select unit
// get paths
// choose target and command
// show targeter
// return
bool AIPlayer::selectNext() {
	auto u = getNext();
	if (u) {
		selected = u->getOwner();
		getMaps(u);
#ifdef TEST
		matrix<float> scores(level_.state.board.width(), level_.state.board.height());
		std::vector<preference> state{ preference{preference_type::ANY_ENEMY}, preference{preference_type::BREAK_ROCK, -.1f, true} };
		for (int y = 0;y < scores.height();y++) {
			for (int x = 0;x < scores.width();x++) {
				scores.at(x, y) = getCompositeScore(state, maps, { x,y });
			}
		}
		level_.displayDJ(std::move(scores));
#endif
		auto paths = pathFind(selected->uc(), level_.state, getAttack(u->stats().attack_type));
		targetFromMap(selected, paths);

		command->showTargeter();
		return true;
	}
	return false;
}

void AIPlayer::executeSelected(sf::Time now) {
	auto state = command->execute(target, now, this);
	if (!state) setMoveState(selected->uc(), unit::move_state::HAS_ACTED);
	deSelect();
	deleteCommand();
}

bool AIPlayer::hasSelection() { return selected != nullptr; }

//debug function. Get a map to display visually
dj_map& AIPlayer::peekMap() {
	getMaps(getNext());
	auto map = maps.begin();
	return map->second;
}