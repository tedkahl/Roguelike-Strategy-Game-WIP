#pragma once
#include <set>
#include <optional>
#include "Debug.h"
#include "Square.h"
#include "Entity.h"
#include "Data.h"
#include "DataManager.h"
#include "SortedDManager.h"
#include "matrix.h"
#include "Board.h"
#include "Paths.h"
#include "Util.h"
using std::cout;
using std::endl;
class Entity;
class DrawComponent;
class Square;
class Board;

class Level
{
private:
	std::shared_ptr<ResourceManager<sf::Texture>> tm_;
	SortedDManager<DrawComponent> dcomponents;
	DataManager<UnitComponent> units;

public:
	DataManager<Entity> entities;
	Board state;
	Level(std::shared_ptr<ResourceManager<sf::Texture>> tm);
	Entity* addEntity(object_type t, sf::Vector2i coords);
	unsigned addTargeter(const pathsGrid& grid);
	bool removeEntity(Entity* e);
	void removeTargeter(unsigned batch);
	void setEntityPos(Entity* e, sf::Vector2i newpos);
	Entity* entityClickedOn(const sf::RenderWindow& window, sf::Vector2i coords, sf::Vector2i pixel, bool prefer_team = false, int team = 0);
	void setSquares(matrix<char>& WFCOutput);
	void update(sf::Time current);
	void draw(sf::RenderWindow& window);
	void killUnit(UnitComponent* killer, Entity* target);

	std::optional < sf::Vector2i> getCoords(sf::RenderWindow& window, sf::Vector2i pixel);
};


