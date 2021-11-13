#include "Square.h"
#include "ResourceManager.h"
#include "Entity.h"
Square::Square(terrain_type terrain_t, DrawComponent* dcs, sf::Vector2i newpos, matrix<float>& heightmap) : pos(newpos.x, newpos.y), type_(terrain_t), unit_(nullptr) {
	dcs->setSquarePos(newpos, heightmap);
	dcs->updateCoords(newpos, dcs->sortable());
}

bool Square::removeE(Entity* e) {
	std::cerr << "Removing entity at " << pos.x << " " << pos.y << std::endl;
	if (e == unit_) unit_ = nullptr;
	else {
		auto search = std::find(entities.begin(), entities.end(), e);
		if (search == entities.end()) return false;
		entities.erase(search);
	}
	return true;
}

void Square::replaceE(Entity* olde, Entity* newe) {
	if (olde == unit_) unit_ = newe;
	else {
		std::replace(entities.begin(), entities.end(), olde, newe);
	}
}

void Square::addE(Entity* e) {
	std::cerr << "Adding entity at " << pos.x << " " << pos.y << std::endl;
	if (e->uc() && !unit_) unit_ = e;
	else {
		entities.push_back(e);
	}
}
Entity* Square::unit() { return unit_; }
UnitComponent* Square::unit_uc() { return unit_ ? unit_->uc() : nullptr; }
