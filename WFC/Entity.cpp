#include "Entity.h"
#include "DrawComponent.h"
#include "UnitComponent.h"
#include "Square.h"

DrawComponent* BoardEntity::dc() { return dc_; }
void BoardEntity::setDC(DrawComponent* dc) { dc_ = dc; }
UnitComponent* BoardEntity::uc() { return uc_; }
void BoardEntity::setUC(UnitComponent* uc) { uc_ = uc; }

void BoardEntity::set(unsigned type, DrawComponent* dc, UnitComponent* uc, unsigned index) {
	type_ = type;
	dc_ = dc;
	uc_ = uc;
	index_ = index;
	dc_->setOwner(this);
	if (uc_) uc_->setOwner(this);
}

void BoardEntity::setPos(sf::Vector2i newpos, BoardState& state) {
	coords_ = newpos;
	owner_ = &state.board.at(newpos);
	dc_->updateEntityPos(newpos, state);
}


void BoardEntity::updatePointers(BoardEntity& removed) {
	owner_->replaceE(&removed, this);
	dc_->setOwner(this);
	if (uc_) uc_->setOwner(this);
}

Square* BoardEntity::getOwner() const { return owner_; }