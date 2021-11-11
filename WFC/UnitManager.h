#pragma once
#include <ranges>
#include "DataManager.h"
#include "UnitComponent.h"
#include "Team.h"
class UnitManager :public DataManager<UnitComponent>
{
private:

public:
	//std::array<UnitComponent, MAX>& getData() { return data; }
	//auto  matchEnmity(int team, enmity_t type) ->std::ranges::filter_view<std::ranges::ref_view<std::array<UnitComponent, MAX>>, std::function<bool(UnitComponent*)> > {
	//	std::function<bool(UnitComponent*)> fxn = [=](UnitComponent* val) {return getEnmity(team, val) == type;};
	//	return data | std::views::filter(fxn);
	//}
	//template<typename T>
	//auto  getAllies(int team) ->std::ranges::filter_view<std::ranges::ref_view<std::array<UnitComponent, MAX>>, T> {
	//	auto fxn = [=](UnitComponent* val) {return getEnmity(team, val) < enmity_t::NEUTRAL;};
	//	static_assert(is_same_v(decltype(fxn), T));
	//	return data | std::views::filter(fxn);
	//}
};
namespace unit {
	static constexpr auto enemies(int team)
	{
		return [=](UnitComponent& val)->bool {return (getEnmity(team, &val) == enmity_t::ENEMY); };
	};
	static constexpr auto allies(int team)
	{
		return [=](UnitComponent& val)->bool {return (getEnmity(team, &val) < enmity_t::NEUTRAL); };
	};

	static constexpr auto sameTeam(int team)
	{
		return [=](UnitComponent& val)->bool {return (getEnmity(team, &val) == enmity_t::SAME_TEAM); };
	};

	static constexpr auto movable()
	{
		return [=](UnitComponent& val)->bool {return (val.getMoveState() != unit::move_state::HAS_ACTED); };
	};
}