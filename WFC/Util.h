#pragma once
#include <SFML/Graphics.hpp>
#include "Data.h"
#include "Random.h"
#include "DataManager.h"
#include "SortedDManager.h"
#include "UnitComponent.h"

static UnitComponent* makeUnit(DataManager<UnitComponent>& units, int team, object_type t) {
	assert(isUnitOrObj(t));
	switch (t) {
	case object_type::WOLF: {
		return units.declareNew(UnitStats(move_type::WALK, attack_type::MELEE, 1, 6, 10 + roll(1, 3), 4), team);
	}
	case object_type::DUELIST: {
		return units.declareNew(UnitStats(move_type::WALK, attack_type::MELEE, 1, 4, 18 + roll(2, 2), 6), team);
	}
	case object_type::ROCK: {
		return units.declareNew(UnitStats(move_type::WALK, attack_type::NO_ATTACK, 0, 0, 50, 0), team);
	}
	case object_type::CACTUS: {
		return units.declareNew(UnitStats(move_type::WALK, attack_type::NO_ATTACK, 0, 0, 30, 0), team);
	}
	default: assert(false);
	}
	return nullptr;
}
//this is asking for bugs. Manager must automatically handle duplicates. 
static DrawComponent* getObjDC(SortedDManager<DrawComponent>& dcomponents, ResourceManager<sf::Texture>& tm, object_type t, unsigned batch = UINT_MAX) {
	auto search = Data<char>::d()->entityinfo.find(t);
	if (search != Data<char>::d()->entityinfo.end()) {
		auto& [path, offset, rect] = search->second;
		if (batch == UINT_MAX)
			return dcomponents.declareNew(tm.get(path), offset, getDefaultOrder(t), rect);
		else
			return dcomponents.declareNewBatched(tm.get(path), offset, getDefaultOrder(t), rect, batch);
	}
	std::cerr << "sprite information not found";
	assert(false);
	return nullptr;
}



