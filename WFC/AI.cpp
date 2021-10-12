#include "AI.h"
#include "Attacks.h"

//bool operator<(const sf::Vector2i& l, const sf::Vector2i& r) { return std::tie(l.x, l.y) < std::tie(r.x, r.y); }
auto actualTargets(matrix<dj_map_node>& dj_map, std::vector<sf::Vector2i>& move_targets) {
	auto cmp = [](const sf::Vector2i& lhs, const sf::Vector2i& rhs) {return compV2i(lhs, rhs);};
	std::set<sf::Vector2i, decltype(cmp)> ret;
	for (auto& i : move_targets) {
		for (auto j : dj_map.at(i).reachable_targets) {
			ret.insert(j);
		}
	}
	return ret;
}
void AIPlayer::decideFinalTarget(Entity* e, matrix<dj_map_node>& dj_map, std::vector<sf::Vector2i>& move_targets) {
	auto t = actualTargets(dj_map, move_targets);
	command.reset(new AttackMove(e, level_));
	target = *t.begin();
}

//select the best move locations from djikstra map
static std::vector<sf::Vector2i> moveTargetsFromMap(Entity* me, matrix<dj_map_node>& dj_map, pathsGrid& paths, Board& board) {
	auto pos = me->getPos();
	int min_score = INT_MAX;
	std::vector<sf::Vector2i> move_targets(1, pos);
	sf::Vector2i sq;
	for (int i = 0;i < paths.grid.width();i++) {
		for (int j = 0;j < paths.grid.height();j++) {
			if (paths.grid.at(i, j).search == paths.search) {
				sq = sf::Vector2i{ i,j } + paths.offset;

				if (paths.grid.at(i, j).movable && !paths.grid.at(i, j).has_ally) {
					if (dj_map.at(sq).moves_left < min_score) {
						move_targets = { sq };
						min_score = dj_map.at(sq).moves_left;
					}
					else if (dj_map.at(sq).moves_left == min_score) {
						move_targets.push_back(sq);
					}
				}
			}
		}
	}
	return move_targets;
}

AIPlayer::AIPlayer(Level& level, int team) :PlayerState(team), level_(level), dj_map(nullptr) {

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
		auto paths = pathFind(selected->uc(), level_.state, getAttack<map_node>(u->stats().attack_type));
		std::vector<sf::Vector2i> targets = moveTargetsFromMap(selected, *dj_map, paths, level_.state);
		decideFinalTarget(selected, *dj_map, targets);
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

//initializing dj map this way not really satisfactory but should work for now
void AIPlayer::getMap(move_type type) {
	dj_map = &djikstraMap(type, team_, enemies, level_.state, getAttack<dj_map_node>(attack_type::MELEE));
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
	p_start_turn(level_.units);
	getMap(move_type::WALK);
}
