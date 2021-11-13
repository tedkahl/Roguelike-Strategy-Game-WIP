#pragma once
#include <SFML/Graphics.hpp>
#include <deque>
#include "type_enums.h"
#include "Managed.h"
#include "DrawComponent.h"
#include "DCSortable.h"
#include "RealTime.h"
#include "GridMove.h"
#include "RTAction.h"
//#include "Data.h"
class DrawComponent;
struct Board;
class UnitComponent;
class Square;
class RealTime;
class DCSortable;

template<typename, typename = void>
constexpr bool is_type_complete_v = false;

template<typename T>
constexpr bool is_type_complete_v
<T, std::void_t<decltype(sizeof(T))>> = true;

class Entity : public Managed
{
private:
	sf::Vector2i coords_;
	Square* owner_;
	DCManager* manager;
	DCSortable dc_;
	UnitComponent* uc_;
	object_type type_;
	std::deque<std::unique_ptr<RealTime >> rt_actions;
public:
	object_type type() const {
		static_assert(is_type_complete_v<RealTime>);
		return type_;
	};
	DrawComponent* dc() const;
	void setDC(DCSortable& dc);
	UnitComponent* uc() const;
	void setUC(UnitComponent* dc);
	void setOwner(Square* new_owner);
	Square* getOwner() const;
	void addRT(std::unique_ptr<RealTime>&& rt);
	bool update(sf::Time current);
	void updatePointers(Entity& removed);
	void set(object_type type, SortedDManager<DrawComponent>* m, DrawComponent* dcs, UnitComponent* uc, sf::Vector2i newpos, Board& state, unsigned index); //datamanager
	inline sf::Vector2i getPos() const { return coords_; }
	//Entity() = default;
	void setPos(sf::Vector2i newpos, Board& state);
	Entity(Entity&& other) noexcept;
	Entity& operator=(Entity&& other) noexcept;
	Entity() = default;
	Entity(const Entity&) = delete;
	friend void swap(Entity& a, Entity& b);
};

