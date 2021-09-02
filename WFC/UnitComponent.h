#pragma once
#include <SFML/Graphics.hpp>
#include "Managed.h"

class BoardEntity;
struct UnitStats {
	unsigned movetype;
	int movement;
	int max_hp;
	int damage;
};


class UnitComponent : public Managed {
private:
	UnitStats stats_;
	int current_hp;
	bool dummy_;
	int team_;
	BoardEntity* owner_;
public:
	inline bool dummy() { return dummy_; }
	inline int team() { return team_; }
	inline int hp() { return current_hp; }
	sf::Vector2i getPos();
	BoardEntity* getOwner();
	void setOwner(BoardEntity* owner);
	UnitStats const& stats() const { return stats_; };
	UnitComponent() = default;
	//UnitComponent(UnitStats stats, int team) :stats_(stats), team_(team), current_hp(stats_.max_hp), Managed(index) {}
	void set(const UnitStats& stats, int team, unsigned index) { stats_ = stats; team_ = team; current_hp = stats_.max_hp; index_ = index; }
	void updatePointers(UnitComponent& removed);
};

