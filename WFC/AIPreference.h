#pragma once
#include "Board.h"
#include <vector>
#include <SFML/Graphics.hpp>
#include "UnitComponent.h"
#include "DataManager.h"
#include <concepts>
#include "type_enums.h"
#include <functional>
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

struct preference {
public:
	preference_type type_;
	float multiplier_;
	bool additive_;
	preference(preference_type type, float multiplier = 1.f, bool additive = false) :type_(type), multiplier_(multiplier), additive_(additive) {}
};

typedef std::function<void(getTargetInput input, std::vector<map_target>&)> TargetFxn;

//filter a list of possible targets according to predicate "p", and score them according to preference function "pref". Store the results in target vector
template<typename T>
requires requires(T t) { t->getPos(); }
const static std::function <void(std::vector<T*>&)> filter = [](std::vector<T*>& possible_targets, std::vector<map_target>& targets, Predicate p = TrueFxn, PrefFxn<T> pref = OneFxn) {
	for (unsigned i = 0;i < possible_targets.size();i++) {
		if (p(possible_targets[i]))
			targets.push_back({ possible_targets[i]->getPos(), pref(possible_targets[i]) });
	}
};
//get list of targets from a preference and current state. Return whether the corresponding map must be updated/recalculated
//space for optimization here: store the changed values to avoid recalculating map from scratch. 
static bool getTargets(preference_type type, getTargetInput input, std::vector<map_target>& targets) {
	std::vector<map_target> new_targets;
	new_targets.reserve(targets.size());
	target_function(type)(input, new_targets);
	bool dirty = (new_targets != targets);
	targets = new_targets;
	return dirty;
}

//could just store lambdas in an array
TargetFxn target_function(preference_type type) {
	switch (type) {
	case preference_type::ANY_ENEMY:
		return [](getTargetInput input, std::vector<map_target>& targets) {
			filter<UnitComponent>(input.enemies, targets);
		};
		break;
	case preference_type::LOW_HP_ENEMY:
		return [](getTargetInput input, std::vector<map_target>& targets) {
			filter<UnitComponent>(input.enemies, targets, TrueFxn, preferLowHP);
		};
		break;
	case preference_type::LOW_HP_ALLY:
		return [](getTargetInput input, std::vector<map_target>& targets) {
			filter<UnitComponent>(input.allies, targets, TrueFxn, preferLowHP);
		};
		break;
	case preference_type::BREAK_ROCK:
		return [](getTargetInput input, std::vector<map_target>& targets) {
			filter<UnitComponent>(input.entities, targets, PickType<Entity, object_type::ROCK>);
		};
	}
}