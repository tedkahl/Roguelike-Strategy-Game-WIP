#include "Team.h"
Alliance::Alliance() :enmity({ {enmity::SAME_TEAM,enmity::ENEMY}, {enmity::ENEMY,enmity::SAME_TEAM} }), teams({ 0,1 }) {}

int Alliance::addAlliance(std::vector<int>&& new_relations) {
	size_t new_index = enmity.size();
	assert(new_relations.size() == new_index);
	for (int i = 0;i < new_relations.size();i++) {
		enmity[i].push_back(new_relations[i]); //enmity, allyship should be symmetrical for now
	}
	new_relations.push_back(enmity::SAME_TEAM);
	enmity.push_back(std::move(new_relations));

	return new_index;
}