#pragma once
#include "BoardState.h"
#include "DrawComponent.h"
#include "Data.h"
class DrawComponent;
struct BoardState;

class BoardEntity
{
private:
	std::string name_;
	std::pair<unsigned, unsigned> coords_;
	unsigned index_;
	DrawComponent* dc_;
public:
	DrawComponent* dc();
	void setDC(DrawComponent* dc);
	inline unsigned index() const { return index_; }
	inline void setIndex(unsigned index) { index_ = index; }
	inline std::pair<unsigned, unsigned>  getOwner() const { return coords_; }
	inline std::pair<unsigned, unsigned> getPos() const { return coords_; }
	BoardEntity() = default;
	void setPos(std::pair<unsigned, unsigned> newpos, BoardState& state);
	BoardEntity(DrawComponent* drawcomponent);

	void set(std::string name, DrawComponent* drawcomponent, unsigned index);
};

