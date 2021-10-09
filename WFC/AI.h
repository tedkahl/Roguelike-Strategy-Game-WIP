#pragma once
#include <algorithm>
#include "PlayerState.h"
#include "UnitComponent.h"
#include "DataManager.h"
#include "Team.h"
#include "Paths.h"
#include "Command.h"
//static void sortByCloseness(Entity* me, std::vector<Entity*>& enemies) {
//	auto entityCmp = [=](const Entity*& e1, const Entity*& e2) {
//		return distSq(me, e1) < distSq(me, e2);
//	};
//	std::sort(enemies.begin(), enemies.end(), entityCmp);
//}

//this is actually pretty insufficient when there are changes mid-turn
static sf::Vector2i target_from_map(Entity* me, matrix<dj_map_node>& dj_map) {
	auto pos = me->getPos();

	int start_score = dj_map.at(pos).moves_left;
	auto next_loc = dj_map.at(pos).prev;
	auto next_next = dj_map.at(next_loc).prev;
	while (next_next.x != -1 && start_score - dj_map.at(next_next).moves_left <= me->uc()->stats().movement) {
		cout << start_score - dj_map.at(next_next).moves_left << endl;
		cout << me->uc()->stats().movement << endl;
		next_loc = next_next;
		next_next = dj_map.at(next_next).prev;
	}
	return next_loc;
}

//select the best squares
//can return a list of possible targets and can even be used to construct targeter in theory
static std::vector<sf::Vector2i> target_from_map_2(Entity* me, matrix<dj_map_node>& dj_map, Board& board) {
	auto pos = me->getPos();
	int start_score = dj_map.at(pos).moves_left;
	int width = 1;
	sf::Vector2i d;
	sf::Vector2i next_dir;
	sf::Vector2i sq;
	int min_score = INT_MAX;
	std::vector<sf::Vector2i> targets(1, pos);
	while (width <= me->uc()->stats().movement) {
		for (int i = 0;i < 4;i++) {
			d = dir[i];
			next_dir = dir[(i + 1) % 4];
			sq = pos + d * width;
			while (sq != pos + next_dir * width) {
				sq -= (dir[i] - next_dir);
				if (start_score - dj_map.at(sq).moves_left >= me->uc()->stats().movement && !board.board.at(sq).unit()) {
					if (dj_map.at(sq).moves_left < min_score) {
						targets = { sq };
						min_score = dj_map.at(sq).moves_left;
					}
					else if (dj_map.at(sq).moves_left == min_score) {
						targets.push_back(sq);
					}
				}
			}
		}
		++width;
	}
	return targets;
}


class AIPlayer :public PlayerState {
private:
	bool has_selection;
	Level& level_;
	matrix<dj_map_node>* dj_map;
	void rushClosest(Entity* me, sf::Time now);
	//virtual void getNextMove();
	void getMap(move_type type);
public:
	virtual void startTurn();
	bool hasSelection() { return has_selection; }
	virtual bool selectNext();
	virtual void executeSelected(sf::Time now);
	AIPlayer(Level& level, int team);
};
