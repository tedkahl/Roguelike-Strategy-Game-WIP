#pragma once
#include "Entity.h"
#include "Square.h"
#include "Board.h"
#include "Debug.h"
#include "UnitComponent.h"

DrawComponent* Entity::dc() const { assert(dc_); return manager->get(dc_.sortVal()); }
void Entity::setDC(DCSortable& dc) { dc_ = dc; }
UnitComponent* Entity::uc() const { return uc_; }
void Entity::setUC(UnitComponent* uc) { uc_ = uc; }

void Entity::addRT(std::unique_ptr<RealTime>&& rt) {
	rt_actions.push_back(std::move(rt));
}
bool Entity::update(sf::Time current) {
	bool dirty = false;
	if (!rt_actions.empty()) {
		//cout << "performing action" << endl;

		auto update = rt_actions.front()->getUpdate(current);
		if (update.action)
			update.action.value()(this);
		manager->updateAll(current, dc_.sortVal(), dc_, update);
		coords_ = dc_.coords();
		if (update.finished) {
			std::cout << "Action finished" << std::endl;
			if (coords_ != owner_->pos) {
				rt_actions.front()->getBoard().moveEntity(this, coords_); //ugly
			}
			rt_actions.pop_front();
		}
		return true;
	}
	return false;

}

void Entity::set(object_type type, SortedDManager<DrawComponent>* m, DrawComponent* dcs, UnitComponent* uc, sf::Vector2i pos, Board& state, unsigned index) {
	type_ = type;
	manager = m;
	uc_ = uc;
	index_ = index;
	rt_actions.clear();

	//set component owners
	dcs->setOwner(this);
	if (uc_) uc_->setOwner(this);

	//set position
	coords_ = pos;
	owner_ = &state.board.at(pos);
	owner_->addE(this);

	//set DrawComponent position 
	dcs->updateEntityPos(pos, state.heightmap);

	//get DrawComponent key
	dc_ = dcs->sortable();
}

void Entity::setPos(sf::Vector2i newpos, Board& state) {
	coords_ = newpos;
	if (owner_)owner_->removeE(this);
	owner_ = &state.board.at(newpos);
	owner_->addE(this);
	dc()->updateEntityPos(newpos, state.heightmap);
}


void Entity::updatePointers(Entity& removed) {
	owner_->replaceE(&removed, this);
	dc()->setOwner(this);
	if (uc_) uc_->setOwner(this);
}

Square* Entity::getOwner() const { return owner_; }
void Entity::setOwner(Square* new_owner) { owner_ = new_owner; }

Entity::Entity(Entity&& other) noexcept {
	coords_ = std::move(other.coords_);
	manager = std::move(other.manager);
	dc_ = std::move(other.dc_);
	uc_ = std::move(other.uc_);
	type_ = other.type_;
	rt_actions = std::move(other.rt_actions);
}
Entity& Entity::operator=(Entity&& other) noexcept {
	coords_ = std::move(other.coords_);
	manager = std::move(other.manager);
	dc_ = std::move(other.dc_);
	uc_ = std::move(other.uc_);
	type_ = other.type_;
	rt_actions = std::move(other.rt_actions);
	return *this;
}

void swap(Entity& a, Entity& b) {
	Entity temp = std::move(a);
	a = std::move(b);
	b = std::move(temp);
}