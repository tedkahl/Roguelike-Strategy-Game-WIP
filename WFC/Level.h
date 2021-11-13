#pragma once
#include <optional>
#include "Debug.h"
#include "AIutil.h"
#include "Square.h"
#include "Entity.h"
#include "Data.h"
#include "DataManager.h"
#include "UnitManager.h"
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

//struct renderBatch {
//	int batch;
//	std::vector<sf::RenderTexture> textures;
//	SortedDManager<DrawComponent>& DCManager;
//	renderBatch(int, std::vector<sf::RenderTexture>&&, SortedDManager<DrawComponent>&);
//	~renderBatch();
//};

//may be nearing the point where we can rethink this class, move data managers somewhere else, etc
//for now don't worry too much about encapsulation I think
class Level
{
private:
	std::shared_ptr<ResourceManager<sf::Texture>> tm_;
	std::shared_ptr<ResourceManager<sf::Font>> fm_;
	std::unique_ptr<SortedDManager<DrawComponent>> dcomponents;
public:
	std::unique_ptr < DataManager<Entity>> entities;
	std::unique_ptr < UnitManager> units;
	Board state;
	explicit Level() {}
	Level(std::shared_ptr<ResourceManager<sf::Texture>> tm);
	Entity* addEntity(object_type t, int team, sf::Vector2i coords);
	Entity* addEntityTest(object_type t, int team, sf::Vector2i coords);
	void addChildDC(object_type type, DrawComponent* parent);
	unsigned addTargeter(const pathsGrid& grid);
	bool removeEntity(Entity* e);
	void removeBatch(unsigned batch);
	Entity* entityClickedOn(const sf::RenderWindow& window, sf::Vector2i coords, sf::Vector2i pixel, bool prefer_team = false, int team = 0);
	void setSquares(matrix<char>& WFCOutput);
	bool update(sf::Time current);
	void draw(sf::RenderWindow& window);
	void killUnit(UnitComponent* killer, Entity* target);
	void displayDJ(dj_map& test);

	std::optional < sf::Vector2i> getCoords(sf::RenderWindow& window, sf::Vector2i pixel);
};


