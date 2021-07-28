#pragma once
#include "Drawable.h"
#include "Data.h"
#include "matrix.h"

class BoardEntity
{
private:
	std::string name;
	std::pair<unsigned, unsigned> coords;
	DrawComponent* dc_;
public:
	DrawComponent* dc() { return dc_; }
	inline std::pair<unsigned, unsigned> getPos() { return coords; }
	BoardEntity() = default;
	inline void setPos(std::pair<unsigned, unsigned> newpos);
	BoardEntity(std::string n, std::string path, sf::Vector2f offset, ResourceManager<sf::Texture>& tm_);
};

BoardEntity::BoardEntity(std::string n, std::string path, sf::Vector2f offset, ResourceManager<sf::Texture>& tm_) : coords(0, 0) {
	dc->set(path, offset, tm_);
}

void BoardEntity::setPos(std::pair<unsigned, unsigned> newpos) {
	coords = newpos;
}
