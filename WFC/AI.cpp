#include "AI.h"
#include "Attacks.h"
#include "Team.h"

//just for movement right now
float getCompositeScore(std::vector<preference>& state, const std::map<preference_type, dj_map>& maps, sf::Vector2i loc) {
	std::partition(state.begin(), state.end(), [](const preference& p) {return !p.additive_;});
	float score = FLT_MAX;
	for (auto& i : state) {
		auto& node = maps.at(i.type_).map.at(loc);
		if (!i.additive_) {
			score = std::min(node.score, score);
		}
		else
			score += node.score;
	}
	return score;
}

//select the best move locations from djikstra map
static sf::Vector2i targetFromMap(Entity* me, const std::map<preference_type, dj_map>& maps, pathsGrid& paths, Board& board) {
	auto pos = me->getPos();
	float min_score = FLT_MAX;
	sf::Vector2i target;
	sf::Vector2i sq;
	std::vector<preference> state{ preference{preference_type::ANY_ENEMY}, preference{preference_type::BREAK_ROCK, -.1f, true} };

	for (int i = 0;i < paths.grid.height();i++) {
		for (int j = 0;j < paths.grid.width();j++) {
			if (paths.grid.at(j, i).search == paths.search) {
				sq = sf::Vector2i{ j,i } + paths.offset;
				auto new_score = (getCompositeScore(state, maps, sq));
				if (paths.grid.at(j, i).movable || paths.grid.at(j, i).attackable && new_score < min_score) {
					target = sq;
					min_score = new_score;
				}
			}
		}
	}
	return target;
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
		auto paths = pathFind(selected->uc(), level_.state, getAttack(u->stats().attack_type));
		auto target = targetFromMap(selected, maps, paths, level_.state);

		command->showTargeter();
		return true;
	}
	return false;
}

void AIPlayer::executeSelected(sf::Time now) {
	command->execute(target, now, this);
	deSelect();
	deleteCommand();
}
//dj_map = &djikstraMap(type, team_, enemies, level_.state, getAttack(attack_type::MELEE));

void AIPlayer::printMap(preference_type type) {
	auto& dj_map = maps[type].map;
	for (unsigned i = 0;i < dj_map.height();i++) {
		for (unsigned j = 0;j < dj_map.width();j++) {
			cout << dj_map.at(j, i).score;
			/*if (dj_map.at(j, i).score < 10) cout << '_';
			cout << " ";*/

		}
		cout << endl;
	}
}

bool AIPlayer::hasSelection() { return selected != nullptr; }
//debug function. Get a map to display visually
dj_map& AIPlayer::peekMap() {
	getMaps(getNext());
	auto map = maps.begin();
	printMap(map->first);
	return map->second;
}