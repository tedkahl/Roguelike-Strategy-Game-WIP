#pragma once
#include <SFML/Graphics.hpp>
#include <queue>
#include "Board.h"
#include "Managed.h"
#include "UnitComponent.h"
#include "Square.h"
#include "RealTime.h"
#include "DrawComponent.h"
#include "DCSortable.h"
#include "Debug.h"
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
	sf::Vector2i coords_;
	Square* owner_;
	SortedDManager<DrawComponent>* manager;
	DCSortable dc_;
	UnitComponent* uc_;
	std::queue<std::unique_ptr<RealTime>> rt_actions;
public:
	short type_;
	DrawComponent* dc() const;
	//DCAccessor& dcAccess();
	void setDC(DCSortable& dc);
	UnitComponent* uc() const;
	void setUC(UnitComponent* dc);
	void setOwner(Square* new_owner);
	Square* getOwner() const;
	void addRT(std::unique_ptr<RealTime>&& rt);
	void update(sf::Time current);
	void updatePointers(Entity& removed);
	void set(unsigned type, SortedDManager<DrawComponent>* m, DrawComponent* dcs, UnitComponent* uc, sf::Vector2i newpos, Board& state, unsigned index); //datamanager
	inline sf::Vector2i getPos() const { return coords_; }
	Entity() = default;
	void setPos(sf::Vector2i newpos, Board& state);
	//Entity(DrawComponent* drawcomponent);
};

