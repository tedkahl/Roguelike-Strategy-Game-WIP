#pragma once
#include "Board.h"
#include <vector>
#include <SFML/Graphics.hpp>
#include "UnitComponent.h"
#include "DataManager.h"
#include <functional>
#include <concepts>
enum class preference_type {
	//enemy target preferences
	ANY_ENEMY, LOW_HP_ENEMY,
	//example map objective
	BREAK_ROCK,
	//example ally objective
	LOW_HP_ALLY
};

/*types of preferences:
1. pursue a map objective
2. pursue a particular type of enemy
	-pursue an enemy being attacked by another ally?
	-avoid blocking allies?
3. pursue an ally
*/

struct map_target {
	sf::Vector2i loc;
	float priority = 1.f;
};

struct getTargetInput {
	std::vector<UnitComponent const*>& enemies;
	std::vector<UnitComponent const*>& allies;
	DataManager<Entity const*>& entities;
};

struct preference {
private:

public:
	preference_type type_;
	float multiplier;
	preference(preference_type type_) : {}
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
typedef std::function<void(getTargetInput input, std::vector<map_target>&)> TargetFxn;
struct map_targets_getter {
	//filter a list of possible targets according to predicate "p", and score them according to preference function "pref". Store the results in target vector
	template<typename T>
	requires requires(T t) { t->getPos(); }
	const static std::function <void(std::vector<T*>&)> filter = [](std::vector<T*>& possible_targets, std::vector<map_target>& targets, Predicate p = TrueFxn, PrefFxn<T> pref = OneFxn) {
		for (unsigned i = 0;i < possible_targets.size();i++) {
			if (p(possible_targets[i]))
				targets.push_back({ possible_targets[i]->getPos(), pref(possible_targets[i]) });
		}
	};
	void getTargets(getTargetInput input, std::vector<map_target>& targets) {
		(*target_getter)(input, targets);
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
}