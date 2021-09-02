#pragma once
#include "BoardState.h"
#include "Managed.h"
#include "DrawComponent.h"
#include "UnitComponent.h"
#include "Square.h"
#include "Movement.h"
#include <SFML/Graphics.hpp>
//#include "Data.h"
class DrawComponent;
struct BoardState;
class UnitComponent;
class Square;
class Movement;
class BoardEntity : public Managed
{
private:
	unsigned type_;
	sf::Vector2i coords_;
	Square* owner_;
	DrawComponent* dc_;
	UnitComponent* uc_;
	Movement* movement;
public:
	DrawComponent* dc();
	void setDC(DrawComponent* dc);
	UnitComponent* uc();
	void setUC(UnitComponent* dc);
	Square* getOwner() const; //datamanager
	inline void addMovement(Movement* move) { movement = move; }
	void update(sf::Time current);
	void updatePointers(BoardEntity& removed);
	void set(unsigned type, DrawComponent* dc, UnitComponent* uc, unsigned index); //datamanager
	inline sf::Vector2i getPos() const { return coords_; }
	BoardEntity() = default;
	void setPos(sf::Vector2i newpos, BoardState& state);
	//BoardEntity(DrawComponent* drawcomponent);
};

