#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "ResourceManager.h"
#include "Drawable.h"
#include "Data.h"
#include "Entity.h"


class Square
{
private:
	std::string name;
	std::vector < BoardEntity*> entities;
	std::shared_ptr<DrawComponent> dc_;
public:
	Square() = default;
	inline std::shared_ptr<DrawComponent> dc() { return dc_; }
	bool removeE(BoardEntity* e);
	void addE(BoardEntity* e);
	Square(std::string n, std::string path, sf::Vector2f offset, ResourceManager<sf::Texture>& tm_);
	Square& operator=(const Square& other);
};


Square::Square(std::string n, std::string path, sf::Vector2f offset, ResourceManager<sf::Texture>& tm_) : dc_(std::make_shared<DrawComponent>(path, offset, tm_)) {
}


bool Square::removeE(BoardEntity* e) {
	auto search = std::find(entities.begin(), entities.end(), e);
	if (search != entities.end()) {
		entities.erase(search);
		return true;
	}
	return false;
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


