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
	rt_actions.push(std::move(rt));
}
bool Entity::update(sf::Time current) {

	if (!rt_actions.empty()) {
		//cout << "performing action" << endl;
		DrawComponent* dc_p = dc();

		auto update = rt_actions.front()->getUpdate(current);
		if (update.move_dir) {
			//cout << "updating move dir" << endl;
			dc_p->setMoveDirection(update.move_dir.value());
			dc_.setMoveDirection(update.move_dir.value());
			if (update.first)
				dc_p->setAnimation(current, rt_actions.front()->getAnimSegs());
		}

		if (update.coords_ && update.coords_.value() != coords_) {
			cout << "updating coords" << endl;
			dc_p->updateCoords(update.coords_.value());
			dc_.updateCoords(update.coords_.value());
			coords_ = update.coords_.value();
			std::cout << to_string(coords_) << " ";
		}
		if (update.sprite_position_) {
			//cout << "setting sprite pos " << to_string(update.sprite_position_.value()) << endl;
			dc_p->setSpritePos(update.sprite_position_.value());
		}
		if (update.action)
			update.action.value()(this);

		if (update.finished) {
			std::cout << "Action finished" << std::endl;
			if (coords_ != owner_->pos)
				rt_actions.front()->getBoard().moveEntity(this, coords_); //bad
			rt_actions.pop();
		}
		cout << "fixing changed values" << endl;
		manager->fixChangedVal(dc_p);
		return true;
	}
	return false;

}

void Entity::set(object_type type, SortedDManager<DrawComponent>* m, DrawComponent* dcs, UnitComponent* uc, sf::Vector2i pos, Board& state, unsigned index) {
	type_ = type;
	manager = m;
	uc_ = uc;
	index_ = index;
	while (!rt_actions.empty()) rt_actions.pop();

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