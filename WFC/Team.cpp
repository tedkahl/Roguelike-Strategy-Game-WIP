#include "Team.h"
Alliance::Alliance() :enmity({ {enmity_t::SAME_TEAM,enmity_t::ENEMY, enmity_t::NEUTRAL}, {enmity_t::ENEMY,enmity_t::SAME_TEAM, enmity_t::NEUTRAL},{enmity_t::SAME_TEAM,enmity_t::NEUTRAL, enmity_t::NEUTRAL} }), teams({ 0,1,2 }) {}

int Alliance::addAlliance(std::vector<enmity_t>&& new_relations) {
	size_t new_index = enmity.size();
	assert(new_relations.size() == new_index);
	for (int i = 0;i < new_relations.size();i++) {
		enmity[i].push_back(new_relations[i]); //enmity, allyship should be symmetrical for now
	}
	new_relations.push_back(enmity_t::SAME_TEAM);
	enmity.push_back(std::move(new_relations));

	return new_index;
}