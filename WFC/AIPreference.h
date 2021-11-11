#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <concepts>
#include <functional>
#include "Board.h"
#include "UnitComponent.h"
#include "DataManager.h"
#include "UnitManager.h"
#include "type_enums.h"
#include "AIutil.h"
/*types of preferences:
1. pursue a map objective
2. pursue a particular type of enemy
	-pursue an enemy being attacked by another ally?
3. stay near an ally
4. avoid "dangerous" enemies
5. block LoS or avoid blocking it
*/
enum class preference_type {
	//enemy target preferences
	ANY_ENEMY, LOW_HP_ENEMY,
	//example map objective
	BREAK_ROCK,
	//example ally objective
	LOW_HP_ALLY
};
//typedef std::function<void(getTargetInput input, std::vector<map_target>&)> TargetFxn;
template <typename T>
using PrefFxn = std::function<float(T&)>;

template<typename T>
static bool TrueFxn(const T& t) {
	return true;
}

template<typename T, object_type type>
//requires requires(T t) { {t.type()}->std::same_as<object_type>; }
static bool PickType(const T& t) {
	return t.type() == type;
}

template<typename T>
static float OneFxn(const T& t) {
	return 1.f;
}

static float preferLowHP(const UnitComponent& unit) {
	return .5 + (static_cast<float>(unit.hp()) / unit.stats().max_hp);
}
//lower multiplier (more negative for additive maps) means stronger preference
struct preference {
public:
	preference_type type_;
	float multiplier_;
	bool additive_;
	preference(preference_type type, float multiplier = 1.f, bool additive = false) :type_(type), multiplier_(multiplier), additive_(additive) {}
};


void findTargets(preference_type type, int team, UnitManager& units, DataManager<Entity>& entities, std::vector<map_target>& targets);
//get list of targets from a preference and current state. Return whether the corresponding map must be updated/recalculated
//space for optimization here: store the changed values to avoid recalculating map from scratch. 
static bool getTargets(preference_type type, int team, UnitManager& units, DataManager<Entity>& entities, std::vector<map_target>& targets) {
	std::vector<map_target> new_targets;
	new_targets.reserve(targets.size());
	findTargets(type, team, units, entities, new_targets);
	bool dirty = (new_targets != targets);
	targets = new_targets;
	return dirty;
}
