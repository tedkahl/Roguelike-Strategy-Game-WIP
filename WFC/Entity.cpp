#include "Entity.h"

DrawComponent* BoardEntity::dc() { return dc_; }
void BoardEntity::setDC(DrawComponent* dc) { dc_ = dc; }

void BoardEntity::set(std::string name, DrawComponent* drawcomponent, unsigned index) {
	name_ = name;
	dc_ = drawcomponent;
	index_ = index;
	dc_->setOwner(this);
}

void BoardEntity::setPos(std::pair<unsigned, unsigned> newpos, BoardState& state) {
	coords_ = newpos;
	dc_->updatePos(newpos, state);
}