#pragma once
#include <functional>
#include <vector>
#include "assert.h"
enum enmity {
	ENMITY_NULL, SAME_TEAM, ALLY, NEUTRAL, ENEMY
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
	inline int numTeams() const { return teams.size(); }

	int getEnmity(int team1, int team2) {
		return enmity[teams[team1]][teams[team2]];
	}
	//accepts teams or pointers to objects with a team() function
};

static constexpr int getEnmity(hasTeam auto u1, hasTeam auto u2)
{
	return (u1 == nullptr || u2 == nullptr) ? enmity::ENMITY_NULL : Alliance::instance()->getEnmity(u1->team(), u2->team());
}

static constexpr int getEnmity(int t1, hasTeam auto u2)
{
	return (!u2) ? enmity::ENMITY_NULL : Alliance::instance()->getEnmity(t1, u2->team());
}

