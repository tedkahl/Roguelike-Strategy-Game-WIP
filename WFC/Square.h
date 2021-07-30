#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "ResourceManager.h"
#include "DrawComponent.h"
#include "assert.h"
#include "Data.h"
#include "Entity.h"

class DrawComponent;
class BoardEntity;
class Square
{
private:
	std::string name;
	std::shared_ptr<DrawComponent> dc_;
public:
	std::vector <BoardEntity*> entities;
	Square() = default;
	std::shared_ptr<DrawComponent> dc();
	bool removeE(BoardEntity* e);
	void addE(BoardEntity* e);
	void replaceE(BoardEntity* olde, BoardEntity* newe);
	//std::vector<BoardEntity*>& getEntities() { return entities; }
	Square(std::string n, std::string path, sf::Vector2f offset, std::shared_ptr<ResourceManager<sf::Texture>> tm);
	Square& operator=(const Square& other);
};





