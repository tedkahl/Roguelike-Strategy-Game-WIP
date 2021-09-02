#include "Entity.h"

DrawComponent* BoardEntity::dc() { return dc_; }
void BoardEntity::setDC(DrawComponent* dc) { dc_ = dc; }
UnitComponent* BoardEntity::uc() { return uc_; }
void BoardEntity::setUC(UnitComponent* uc) { uc_ = uc; }


void BoardEntity::update(sf::Time current) {
	if (movement) {
		if (movement->isFinished(current)) delete movement;
		else {
			coords_ = movement->getCoordChange(current);
			dc_->setSpritePos(movement->getSpritePos(current));
		}
	}
}
void BoardEntity::set(unsigned type, DrawComponent* dc, UnitComponent* uc, unsigned index) {
	type_ = type;
	dc_ = dc;
	uc_ = uc;
	index_ = index;
	movement = nullptr;
	dc_->setOwner(this);
	if (uc_) uc_->setOwner(this);
}

void BoardEntity::setPos(sf::Vector2i newpos, BoardState& state) {
	coords_ = newpos;
	if (owner_)owner_->removeE(this);
	owner_ = &state.board.at(newpos);
	owner_->addE(this);
	dc_->updateEntityPos(newpos, state);
}


void BoardEntity::updatePointers(BoardEntity& removed) {
	owner_->replaceE(&removed, this);
	dc_->setOwner(this);
	if (uc_) uc_->setOwner(this);
}

Square* BoardEntity::getOwner() const { return owner_; }