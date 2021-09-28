#pragma once
#include <SFML/Graphics.hpp>
#include "Board.h"
#include "Managed.h"
#include "UnitComponent.h"
#include "Square.h"
#include "RealTime.h"
#include "DrawComponent.h"
#include "DCSortable.h"
//#include "Data.h"
class DrawComponent;
struct Board;
class UnitComponent;
class Square;
class RealTime;
class DCSortable;
class Entity : public Managed
{
private:
	unsigned type_;
	sf::Vector2i coords_;
	Square* owner_;
	SortedDManager<DrawComponent>* manager;
	DCSortable dc_;
	UnitComponent* uc_;
	std::unique_ptr<RealTime> movement;
public:
	DrawComponent* dc() const;
	//DCAccessor& dcAccess();
	void setDC(DCSortable& dc);
	UnitComponent* uc() const;
	void setUC(UnitComponent* dc);
	void setOwner(Square* new_owner);
	Square* getOwner() const;
	void addMovement(std::unique_ptr<RealTime>&& move);
	void update(sf::Time current);
	void updatePointers(Entity& removed);
	void set(unsigned type, SortedDManager<DrawComponent>* m, DrawComponent* dcs, UnitComponent* uc, sf::Vector2i newpos, Board& state, unsigned index); //datamanager
	inline sf::Vector2i getPos() const { return coords_; }
	Entity() = default;
	void setPos(sf::Vector2i newpos, Board& state);
	//Entity(DrawComponent* drawcomponent);
};

