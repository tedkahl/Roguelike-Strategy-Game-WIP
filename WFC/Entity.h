#pragma once
#include "BoardState.h"
#include "DrawComponent.h"
#include "UnitComponent.h"
#include "Square.h"
#include "Data.h"
class DrawComponent;
struct BoardState;
class UnitComponent;
class Square;
class BoardEntity : public Managed
{
private:
	unsigned type_;
	std::pair<unsigned, unsigned> coords_;
	Square* owner_;
	DrawComponent* dc_;
	UnitComponent* uc_;
public:
	DrawComponent* dc();
	void setDC(DrawComponent* dc);
	UnitComponent* uc();
	void setUC(UnitComponent* dc);
	Square* getOwner() const; //datamanager
	void updatePointers(BoardEntity& removed);
	void set(unsigned type, DrawComponent* dc, UnitComponent* uc, unsigned index); //datamanager
	inline std::pair<unsigned, unsigned> getPos() const { return coords_; }
	BoardEntity() = default;
	void setPos(std::pair<unsigned, unsigned> newpos, BoardState& state);
	//BoardEntity(DrawComponent* drawcomponent);
};

