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
//0 no active action
//1 current active action
//2 action finished, delete this entity after updates
uint8_t Entity::update(sf::Time current) {
	if (!rt_actions.empty()) {
		uint8_t ret = 1;
		//cout << "performing action" << endl;

		auto update = rt_actions.front()->getUpdate(current);
		manager->updateAll(current, dc_.sortVal(), dc_, update);
		coords_ = dc_.coords();
		if (update.finished) {
			std::cout << "Action finished " << (int)update.finished << " queue size: " << rt_actions.size() << std::endl;
			if (coords_ != owner_->getPos()) {
				rt_actions.front()->getBoard().moveEntity(this, coords_); //ugly
			}
			std::cout << " queue size: " << rt_actions.size() << std::endl;
			rt_actions.pop_front();
			if (update.finished == 2) ret = 2; //deactivate
			else ret = 0;
		}
		if (update.action)
			update.action.value()(this);
		return ret;
	}
	return 0;
}

void Entity::set(object_type type, SortedDManager<DrawComponent>* m, DrawComponent* dcs, UnitComponent* uc, sf::Vector2i pos, Board& state, unsigned index) {
	type_ = type;
	manager = m;
	uc_ = uc;
	index_ = index;
	rt_actions.clear();

	//set component owner
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
	if (uc_) uc_->setOwner(this);
}

Square* Entity::getOwner() const { return owner_; }
void Entity::setOwner(Square* new_owner) { owner_ = new_owner; }

Entity::Entity(Entity&& other) noexcept {
	coords_ = std::move(other.coords_);
	manager = std::move(other.manager);
	owner_ = std::move(other.owner_);
	dc_ = std::move(other.dc_);
	uc_ = std::move(other.uc_);
	type_ = other.type_;
	rt_actions = std::move(other.rt_actions);
}
Entity& Entity::operator=(Entity&& other) noexcept {
	coords_ = std::move(other.coords_);
	manager = std::move(other.manager);
	owner_ = std::move(other.owner_);
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