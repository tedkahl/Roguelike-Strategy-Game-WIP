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
	void targetFromMap(Entity* me, pathsGrid& paths);
public:
	void getMaps(UnitComponent* unit);
	dj_map& peekMap();
	bool hasSelection();
	virtual bool selectNext();
	virtual void executeSelected(sf::Time now);
	using Player::Player;
};
