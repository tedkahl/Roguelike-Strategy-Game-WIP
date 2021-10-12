#pragma once
#include <algorithm>
#include "PlayerState.h"
#include "UnitComponent.h"
#include "DataManager.h"
#include "Team.h"
#include "Paths.h"
#include "Command.h"

class AIPlayer :public PlayerState {
private:
	Level& level_;
	matrix<dj_map_node>* dj_map;
	sf::Vector2i target;
	void getMap(move_type type);
public:
	void startTurn();
	virtual void startTurn(DataManager<UnitComponent>& units) override { assert(units.active() == UINT32_MAX); };
	UnitComponent* getNext() { return static_cast<Player*>(this)->getNext(level_.units); }
	bool hasSelection() { return selected == nullptr; }
	virtual bool selectNext();
	virtual void executeSelected(sf::Time now);
	void decideFinalTarget(Entity* e, matrix<dj_map_node>& dj_map, std::vector<sf::Vector2i>& move_targets);
	AIPlayer(Level& level, int team);
};
