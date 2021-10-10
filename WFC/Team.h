#pragma once
#include <functional>
#include <vector>
#include "assert.h"
enum class enmity_t {
	ENMITY_NULL, SAME_TEAM, ALLY, NEUTRAL, ENEMY
};
static constexpr bool isNE(enmity_t a) {
	return static_cast<int>(a) >= static_cast<int>(enmity_t::NEUTRAL);
}
template <typename T>
concept hasTeam = requires(T t)
{
	{t->team()} -> std::same_as<int>;
};


//alliance 0 is always player, alliance 1 is always "generic enemies" 
class Alliance {
private:
	//enmity[a][b]==enmity_t::ENEMY if alliance a and b are enemies
	std::vector<std::vector<enmity_t>> enmity;
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
	int addAlliance(std::vector<enmity_t>&& new_relations);
	//add team to alliance
	int addTeam(int alliance) {
		teams.push_back(alliance);
		return teams.size() - 1;
	}

	void updateEnmity(int alli_a, int alli_b, enmity_t new_enmity) {
		enmity[alli_a][alli_b] = new_enmity;
		enmity[alli_b][alli_a] = new_enmity;
	}
	inline int numTeams() const { return teams.size(); }

	enmity_t getEnmity(int team1, int team2) {
		return enmity[teams[team1]][teams[team2]];
	}
	//accepts teams or pointers to objects with a team() function
};

static enmity_t getEnmity(hasTeam auto u1, hasTeam auto u2)
{
	return (u1 == nullptr || u2 == nullptr) ? enmity_t::ENMITY_NULL : Alliance::instance()->getEnmity(u1->team(), u2->team());
}

static enmity_t getEnmity(int t1, hasTeam auto u2)
{
	return (!u2) ? enmity_t::ENMITY_NULL : Alliance::instance()->getEnmity(t1, u2->team());
}

