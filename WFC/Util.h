#pragma once
#include "Data.h"
#include "Random.h"
#include "DataManager.h"
#include "SortedDManager.h"
#include "UnitComponent.h"

static UnitComponent* makeUnit(DataManager<UnitComponent>& units, int team, object_type t) {
	switch (t) {
	case object_type::WOLF: {
		return units.declareNew(UnitStats(move_type::WALK, attack_type::MELEE, 1, 6, 10 + roll(1, 3), 4), team);
	}
	case object_type::DUELIST: {
		return units.declareNew(UnitStats(move_type::WALK, attack_type::MELEE, 1, 4, 18 + roll(2, 2), 6), team);
	}
	case object_type::ROCK: {
		return units.declareNew(UnitStats(move_type::WALK, attack_type::NO_ATTACK, 0, 0, 50, 0), 100);
	}
	case object_type::CACTUS: {
		return units.declareNew(UnitStats(move_type::WALK, attack_type::NO_ATTACK, 0, 0, 30, 0), 100);
	}
	default: return nullptr;
	}
}

static DrawComponent* getObjDC(SortedDManager<DrawComponent>& dcomponents, std::shared_ptr<ResourceManager<sf::Texture>> tm, object_type t) {
	auto search = Data<char>::d()->entityinfo.find(t);
	if (search != Data<char>::d()->entityinfo.end()) {
		auto& [path, offset, rect] = search->second;
		return dcomponents.declareNew(path, offset, tm, 1 + isTall(t), rect);
	}
	return nullptr;
}

static DrawComponent* getObjDCBatched(SortedDManager<DrawComponent>& dcomponents, std::shared_ptr<ResourceManager<sf::Texture>> tm, object_type t, unsigned batch) {
	auto search = Data<char>::d()->entityinfo.find(t);
	if (search != Data<char>::d()->entityinfo.end()) {
		auto& [path, offset, rect] = search->second;
		return dcomponents.declareNewBatched(path, offset, tm, 1 + isTall(t), rect, batch);
	}
	return nullptr;
}



