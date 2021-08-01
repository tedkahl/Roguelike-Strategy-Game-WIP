#include "Entity.h"

DrawComponent* BoardEntity::dc() { return dc_; }
void BoardEntity::setDC(DrawComponent* dc) { std::cout << "calling setDC(entity)\n";dc_ = dc; }

void BoardEntity::set(unsigned type, DrawComponent* drawcomponent, unsigned index) {
	type_ = type;
	dc_ = drawcomponent;
	index_ = index;
	dc_->setOwner(this);
}

void BoardEntity::setPos(std::pair<unsigned, unsigned> newpos, BoardState& state) {
	coords_ = newpos;
	dc_->updateEntityPos(newpos, state);
}