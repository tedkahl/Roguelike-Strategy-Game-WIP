#include "Square.h"
Square::Square(terrain_type t, DrawComponent* dc) : type(t), dc_(dc) {
	dc_->setOwner(this);
}

DrawComponent* Square::dc() const { return dc_; }

void Square::setDC(DrawComponent* dc) { std::cout << "calling setDC(square)\n";dc_ = dc; }


bool Square::removeE(BoardEntity* e) {
	auto search = std::find(entities.begin(), entities.end(), e);
	if (search != entities.end()) {
		entities.erase(search);
		return true;
	}
	return false;
}

void Square::replaceE(BoardEntity* olde, BoardEntity* newe) {
	assert(std::find(entities.begin(), entities.end(), olde) != entities.end());

	std::replace(entities.begin(), entities.end(), olde, newe);
}


void Square::addE(BoardEntity* e) {
	entities.push_back(e);
}


Square& Square::operator=(const Square& other)
{
	type = other.type;
	dc_ = other.dc_;
	return *this;
}