#include "Square.h"
Square::Square(std::string n, std::string path, sf::Vector2f offset, std::shared_ptr<ResourceManager<sf::Texture>> tm) : dc_(std::make_shared<DrawComponent>(path, offset, tm)) {
}

std::shared_ptr<DrawComponent> Square::dc() { return dc_; }


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
	name = other.name;
	dc_ = other.dc_;
	return *this;
}