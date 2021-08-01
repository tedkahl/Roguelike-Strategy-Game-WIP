#pragma once
#include "BoardState.h"
#include "DrawComponent.h"
#include "Data.h"
class DrawComponent;
struct BoardState;

class BoardEntity
{
private:
	unsigned type_;
	std::pair<unsigned, unsigned> coords_;
	unsigned index_;
	DrawComponent* dc_;
public:
	DrawComponent* dc();
	void setDC(DrawComponent* dc);
	inline unsigned index() const { return index_; } //datamanager
	inline void setIndex(unsigned index) { index_ = index; } //datamanager
	inline std::pair<unsigned, unsigned>  getOwner() const { return coords_; } //datamanager
	void set(unsigned type, DrawComponent* drawcomponent, unsigned index); //datamanager
	inline std::pair<unsigned, unsigned> getPos() const { return coords_; }
	BoardEntity() = default;
	void setPos(std::pair<unsigned, unsigned> newpos, BoardState& state);
	BoardEntity(DrawComponent* drawcomponent);
};


struct UnitStats {
	int movement;
	int max_hp;
	int current_hp;
};

class Unit : public BoardEntity
{
private:
	UnitStats stats;
public:
	UnitStats const& getStats() const { return stats; };

};
