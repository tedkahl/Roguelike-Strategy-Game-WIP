#pragma once
#include <SFML/Graphics.hpp>
#include "type_enums.h"
#include <functional>
#include "UnitComponent.h"
#include "DataManager.h"


struct map_target {
	sf::Vector2i loc;
	float priority = 1.f;
};
struct getTargetInput {
	std::vector<UnitComponent*>& enemies;
	std::vector<UnitComponent*>& allies;
	DataManager<Entity>& entities;
};
/* Return the relative value placed on a target. 1 is average, lower is better
*/
typedef std::function<void(getTargetInput input, std::vector<map_target>&)> TargetFxn;
template <typename T>
using PrefFxn = std::function<float(T*)>;
template <typename T>
using Predicate = std::function<bool(T*)>;

template<typename T>
static bool TrueFxn(T* t) {
	return true;
}

template<typename T, object_type type>
requires requires(T t) { {t.type()}->std::same_as<object_type>; }
static bool PickType(T* t) {
	return t->type() == type;
}

template<typename T>
static float OneFxn(T* t) {
	return 1.f;
}

static float preferLowHP(UnitComponent* unit) {
	return .5 + (static_cast<float>(unit->hp()) / unit->stats().max_hp);
}