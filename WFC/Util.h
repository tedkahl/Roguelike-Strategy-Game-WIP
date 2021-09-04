#pragma once
#include "Data.h"
#include "Random.h"
#include "DataManager.h"
#include "UnitComponent.h"

static UnitComponent* makeUnit(DataManager<UnitComponent>& units, int team, object_type t) {
	switch (t) {
	case object_type::WOLF: {
		return units.declareNew(UnitStats(move_type::WALK, 6, 10 + roll(1, 3), 4), team);
	}
	case object_type::DUELIST: {
		return units.declareNew(UnitStats(move_type::WALK, 4, 18 + roll(2, 2), 6), team);
	}
	default: return nullptr;
	}
}

static DrawComponent* getObjDC(DataManager<DrawComponent>& dcomponents, std::shared_ptr<ResourceManager<sf::Texture>> tm, object_type t) {
	auto search = Data<char>::d()->entityinfo.find(t);
	if (search != Data<char>::d()->entityinfo.end()) {
		auto& [path, offset, rect] = search->second;
		return dcomponents.declareNew(path, offset, tm, 5, rect);
	}
	return nullptr;
}

static constexpr bool isUnit(object_type t) {
	return t >= firstunit;
}

