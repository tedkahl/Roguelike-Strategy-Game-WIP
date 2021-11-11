#pragma once
#include <algorithm>
#include <map>
#include "Player.h"
#include "UnitComponent.h"
#include "DataManager.h"
#include "AIComponent.h"
#include "AIPreference.h"
#include "AIutil.h"
#include "Paths.h"
#include "Command.h"
class AIPlayer :public Player {
private:
	int turn;
	std::map<preference_type, dj_map> maps;
	sf::Vector2i target;
	void printMap(preference_type type);
public:
	void getMaps(UnitComponent* unit);
	dj_map& peekMap();
	bool hasSelection();
	virtual bool selectNext();
	virtual void executeSelected(sf::Time now);
	//void decideFinalTarget(Entity* e, matrix<dj_map_node>& dj_map, std::vector<sf::Vector2i>& move_targets);
	using Player::Player;
};
