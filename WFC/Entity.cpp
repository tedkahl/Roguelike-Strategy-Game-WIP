#include "Entity.h"

DrawComponent* Entity::dc() const { assert(dc_); return manager->get(dc_.sortVal()); }
void Entity::setDC(DCSortable& dc) { dc_ = dc; }
UnitComponent* Entity::uc() const { return uc_; }
void Entity::setUC(UnitComponent* uc) { uc_ = uc; }

void Entity::addMovement(std::unique_ptr<RealTime>&& move) {
	movement = std::move(move);
	DrawComponent* dc_p = dc();
	dc_p->setMoveDirection(0);
	dc_.setMoveDirection(0);
	manager->fixChangedVal(dc_p);
}
void Entity::update(sf::Time current) {
	if (movement) {
		//std::cout << "updating entity at pos " << coords_.x << " " << coords_.y << std::endl;
		DrawComponent* dc_p = dc();
		auto update = movement->getUpdate(current);

		if (update.coords_ && update.coords_.value() != coords_) {
			dc_p->updateCoords(update.coords_.value());
			dc_.updateCoords(update.coords_.value());
			coords_ = update.coords_.value();
		}

		if (update.finished) {
			std::cout << "Movement finished" << std::endl;
			dc_p->setMoveDirection(1);
			dc_.setMoveDirection(1);
			movement->getBoard().moveEntity(this, coords_);
			delete movement.release();
		}
		else {
			if (update.spritePosition_) dc_p->setSpritePos(update.spritePosition_.value());
		}
		manager->fixChangedVal(dc_p);
	}

}

void Entity::set(unsigned type, SortedDManager<DrawComponent>* m, DrawComponent* dcs, UnitComponent* uc, sf::Vector2i pos, Board& state, unsigned index) {
	type_ = type;
	manager = m;
	uc_ = uc;
	index_ = index;
	movement = nullptr;

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