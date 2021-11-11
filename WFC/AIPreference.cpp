#include "AIPreference.h"
#include <ranges>
#include <iterator>
#include <type_traits>
#include "DataManager.h"
#include "UnitManager.h"
using namespace std::views;
template<typename T>
concept hasPosition = requires(T t) { {t.getPos()}->std::same_as<sf::Vector2i>; };


//filter a list of possible targets (units, entities, according to predicate "p", and score them according to preference function "pref". Store the results in target vector
template<typename V, typename T = std::ranges::range_value_t<V>, typename Pred = decltype(TrueFxn<T>), typename Pref = decltype(OneFxn<T>)>
requires std::ranges::view<V>&& hasPosition<T>&& std::predicate<Pred, T&>
static void targetFilter(V&& possible_targets, std::vector<map_target>& targets, Pred p = TrueFxn<T>, Pref pref = OneFxn<T>) {
	for (auto const&& i : (possible_targets | filter(p) | transform([&](auto& val) {
		return map_target(val.getPos(), pref(val));}
	))) {
		targets.push_back(i);
	}
}

static_assert(std::predicate< decltype(PickType<UnitComponent, object_type::ROCK>), UnitComponent&>);

//class std::ranges::transform_view<std::ranges::filter_view<std::ranges::ref_view<std::vector<int>>, type>, type>
void findTargets(preference_type type, int team, UnitManager& units, DataManager<Entity>& entities, std::vector<map_target>& targets)
{

	switch (type) {
	case preference_type::ANY_ENEMY:
		targetFilter(all(units) | filter(unit::enemies(team)), targets);
		break;//take(units.active())|
	case preference_type::LOW_HP_ENEMY:
		targetFilter(all(units) | filter(unit::enemies(team)), targets, TrueFxn<UnitComponent>, preferLowHP);
		break;
	case preference_type::LOW_HP_ALLY:
		targetFilter(all(units) | filter(unit::allies(team)), targets, TrueFxn<UnitComponent>, preferLowHP);
		break;
	case preference_type::BREAK_ROCK:
		targetFilter(all(entities), targets, PickType<Entity, object_type::ROCK>, OneFxn<Entity>);
	}
}