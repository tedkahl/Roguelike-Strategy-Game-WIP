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
	terrain_type type;
	DrawComponent* dc_;
public:
	std::vector <BoardEntity*> entities;
	Square() = default;
	DrawComponent* dc() const;
	void setDC(DrawComponent* dc);
	bool removeE(BoardEntity* e);
	void addE(BoardEntity* e);
	void replaceE(BoardEntity* olde, BoardEntity* newe);
	//std::vector<BoardEntity*>& getEntities() { return entities; }
	Square(terrain_type t, DrawComponent* dc);
	Square& operator=(const Square& other);
};





