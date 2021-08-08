#pragma once
#include "Managed.h"
#include "Entity.h"

class BoardEntity;
struct UnitStats {
	unsigned movetype;
	int movement;
	int max_hp;
	int min_damage;
	int max_damage;
};


class UnitComponent : public Managed {
private:
	UnitStats stats_;
	int current_hp;
	bool dummy_;
	BoardEntity* owner_;
public:
	inline bool dummy() { return dummy_; }
	inline int hp() { return current_hp; }
	BoardEntity* getOwner();
	void setOwner(BoardEntity* owner);
	int team_;
	UnitStats const& stats() const { return stats_; };
	UnitComponent() = default;
	//UnitComponent(UnitStats stats, int team) :stats_(stats), team_(team), current_hp(stats_.max_hp), Managed(index) {}
	void set(UnitStats stats, int team, unsigned index) { stats = stats; team_ = team; current_hp = stats_.max_hp; index_ = index; }
	void updatePointers(UnitComponent& removed);
};

