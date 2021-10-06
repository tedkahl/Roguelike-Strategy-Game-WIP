#pragma once
#include <functional>
#include <vector>
#include "assert.h"
enum enmity {
	SAME_TEAM, ALLY, NEUTRAL, ENEMY
};
template <typename T>
concept hasTeam = requires(T t)
{
	{t->team()} -> std::same_as<int>;
};


//alliance 0 is always player, alliance 1 is always "generic enemies" 
class Alliance {
private:
	//enmity[a][b]==enmity::ENEMY if alliance a and b are enemies
	std::vector<std::vector<int>> enmity;
	//index is team number, value is alliance. Eventually there could/should be more team info
	std::vector<int> teams;

	Alliance();
	inline static std::unique_ptr<Alliance> instance_;
public:
	static Alliance* instance() {
		if (!instance_)
			instance_ = std::unique_ptr<Alliance>(new Alliance);
		return instance_.get();
	}
	//add alliance specifying relations with all existing alliances
	int addAlliance(std::vector<int>&& new_relations);
	//add team to alliance
	int addTeam(int alliance) {
		teams.push_back(alliance);
		return teams.size() - 1;
	}

	void updateEnmity(int alli_a, int alli_b, int new_enmity) {
		enmity[alli_a][alli_b] = new_enmity;
		enmity[alli_b][alli_a] = new_enmity;
	}

	//accepts teams or pointers to objects with a team() function
	template <typename T1, typename T2>
	constexpr int getEnmity(T1 u1, T2 u2)
	{
		int a, b;
		if constexpr (hasTeam<T1>)
			a = u1->team();
		else {
			static_assert(std::is_same_v<T1, int>);
			a = u1;
		}
		if constexpr (hasTeam<T2>)
			b = u2->team();
		else {
			static_assert(std::is_same_v<T2, int>);
			b = u2;
		}
		return enmity[teams[a]][teams[b]];
	}

};
