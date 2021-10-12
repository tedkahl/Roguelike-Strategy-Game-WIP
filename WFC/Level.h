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
#include "Team.h"
#include "Paths.h"
#include "Util.h"
#include "vec2i_util.h"
using std::cout;
using std::endl;
class Entity;
class DrawComponent;
class Square;
struct Board;

//may be nearing the point where we can rethink this class, move data managers somewhere else, etc
//for now don't worry too much about encapsulation I think
class Level
{
private:
	std::shared_ptr<ResourceManager<sf::Texture>> tm_;
	SortedDManager<DrawComponent> dcomponents;
public:
	DataManager<Entity> entities;
	DataManager<UnitComponent> units;
	Board state;
	explicit Level() {}
	Level(std::shared_ptr<ResourceManager<sf::Texture>> tm);
	Entity* addEntity(object_type t, sf::Vector2i coords);
	unsigned addTargeter(const pathsGrid& grid);
	bool removeEntity(Entity* e);
	void removeTargeter(unsigned batch);
	Entity* entityClickedOn(const sf::RenderWindow& window, sf::Vector2i coords, sf::Vector2i pixel, bool prefer_team = false, int team = 0);
	void setSquares(matrix<char>& WFCOutput);
	bool update(sf::Time current);
	void draw(sf::RenderWindow& window);
	void killUnit(UnitComponent* killer, Entity* target);

	std::optional < sf::Vector2i> getCoords(sf::RenderWindow& window, sf::Vector2i pixel);
};


