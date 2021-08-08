#include "Square.h"
Square::Square(unsigned terrain_t, DrawComponent* dc) : type(terrain_t), dc_(dc) {
	dc_->setOwner(this);
}

DrawComponent* Square::dc() const { return dc_; }

void Square::setDC(DrawComponent* dc) { dc_ = dc; }


bool Square::removeE(BoardEntity* e) {
	if (unit_ && e->uc() == unit_) unit_ = nullptr;
	else {
		auto search = std::find(entities.begin(), entities.end(), e);
		if (search == entities.end()) return false;
		entities.erase(search);
	}
	return true;
}

void Square::replaceE(BoardEntity* olde, BoardEntity* newe) {
	if (unit_ && olde->uc() == unit_) unit_ = newe->uc();
	else {
		std::replace(entities.begin(), entities.end(), olde, newe);
	}
}

void Square::addE(BoardEntity* e) {

	if (e->uc() && !unit_) unit_ = e->uc();
	else {
		entities.push_back(e);
	}
}
UnitComponent* Square::unit() { return unit_; }

Square& Square::operator=(const Square& other)
{
	type = other.type;
	dc_ = other.dc_;
	return *this;
}