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
class UnitComponent;
class Square
{
private:
	unsigned type;
	DrawComponent* dc_;
	UnitComponent* unit_;
public:
	std::vector <BoardEntity*> entities;
	Square() = default;
	DrawComponent* dc() const;
	UnitComponent* unit();
	void setDC(DrawComponent* dc);
	bool removeE(BoardEntity* e);
	void addE(BoardEntity* e);
	void replaceE(BoardEntity* olde, BoardEntity* newe);
	//std::vector<BoardEntity*>& getEntities() { return entities; }
	Square(unsigned terrain_t, DrawComponent* dc);
	Square& operator=(const Square& other);
};





