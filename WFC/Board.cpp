#include "Board.h"
#include "pch.h"

//takes an entity whose position vector has been changed.
//removes it from its outdated owner square, adds it to the new one. Returns pointer to new square
void Board::moveEntity(Entity* e, sf::Vector2i newpos) {
	e->getOwner()->removeE(e);
	board.at(newpos).addE(e);
	e->setOwner(&board.at(newpos));
}