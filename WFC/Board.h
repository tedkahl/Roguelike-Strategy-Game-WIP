#pragma once
#include <set>
#include <optional>
#include "Square.h"
#include "Entity.h"
#include "Data.h"
#include "DataManager.h"
#include "matrix.h"
#include "BoardState.h"
#include "Paths.h"
#include "Util.h"
template<typename T>
class Board
{
private:
	std::shared_ptr<sf::RenderTexture> boardTexture;
	std::shared_ptr<ResourceManager<sf::Texture>> tm_;
	DataManager<DrawComponent> dcomponents;
	DataManager<UnitComponent> units;
	//BoardEntity* selected;

	void makeTexture();
public:
	DataManager<BoardEntity> entities;
	BoardState state;
	Board(std::shared_ptr<ResourceManager<sf::Texture>> tm);
	BoardEntity* addEntity(object_type t, sf::Vector2i coords);
	bool removeEntity(BoardEntity* e);
	void setEntityPos(BoardEntity* e, sf::Vector2i newpos);
	void select(UnitComponent* u);
	void setSquares(matrix<T>& WFCOutput);
	void draw(sf::RenderWindow& window);
	std::optional < sf::Vector2i> getCoords(sf::RenderWindow& window, sf::Vector2i pixel);
};

template<typename T>
Board<T>::Board(std::shared_ptr<ResourceManager<sf::Texture>> tm) :tm_(tm) {}

template<typename T>
void Board<T>::setEntityPos(BoardEntity* e, sf::Vector2i newpos) {
	auto coords = e->getPos();
	assert(state.board.at(coords).removeE(e));
	state.board.at(newpos).addE(e);
	e->setPos(newpos);
}
//
//template<typename T>
//Square* getSquare(std::pair<unsigned, unsigned> coords)
//{
//	if (0 <= x_pos && x_pos < state.board.width() && 0 <= y_pos && y_pos < state.board.height())
//		return &state.board.at((unsigned)x_pos, (unsigned)y_pos);
//	return nullptr;
//}

template<typename T>
std::optional<sf::Vector2i> Board<T>::getCoords(sf::RenderWindow& window, sf::Vector2i pixel) {
	auto coords = window.mapPixelToCoords(pixel);

	double x_pos = coords.y + (.5 * coords.x);
	double y_pos = coords.y - (.5 * coords.x);
	x_pos = x_pos / sq::square_h - (float)state.board.height() / 2;
	y_pos = y_pos / sq::square_h + (float)state.board.height() / 2;

	std::cout << "xy: " << x_pos << " " << y_pos << std::endl;
	if (0 <= x_pos && x_pos < state.board.width() && 0 <= y_pos && y_pos < state.board.height())
		return sf::Vector2i(x_pos, y_pos);
	return std::nullopt;
}


template<typename T>
void Board<T>::draw(sf::RenderWindow& window) {

	auto& view = window.getView();
	sf::FloatRect v(view.getCenter() - view.getSize() / 2.0f, view.getSize());
	std::set<DrawComponent> s;
	for (const auto& i : dcomponents) {
		if (i.getSprite().getGlobalBounds().intersects(v))
			s.insert(i);
	}
	for (const auto& i : s) {
		i.draw(&window);
	}
}

template<typename T>
BoardEntity* Board<T>::addEntity(object_type type, sf::Vector2i coords)
{
	UnitComponent* uc = makeUnit(units, 0, type);
	if (uc && state.board.at(coords).unit()) {
		std::cerr << "space already full\n";
		return nullptr;
	}
	DrawComponent* dc = getObjDC(dcomponents, tm_, type);
	BoardEntity* entity = entities.declareNew(type, dc, uc);
	entity->setPos(coords, state);
	return entity;
}

template<typename T>
bool Board<T>::removeEntity(BoardEntity* e)
{
	if (state.board.at(e->getPos()).removeE(e)) {
		dcomponents.deactivate(e->dc()->index());
		entities.deactivate(e->index());
		return true;
	}
	return false;
}


template<typename T>
void Board<T>::setSquares(matrix<T>& WFCOutput)
{
	state.board.set(WFCOutput.width(), WFCOutput.height(), Square());

	for (unsigned h = 0;h < WFCOutput.height();h++) {
		for (unsigned w = 0;w < WFCOutput.width();w++) {
			T val = WFCOutput.at(w, h);
			auto [terrain_t, entity_t] = Data<T>::d()->glyphs.at(val);
			auto [path, offset, rect] = Data<T>::d()->squareinfo.at(terrain_t);
			state.board.at(w, h) = Square(terrain_t, dcomponents.declareNew(path, offset, tm_, 0, rect));
			state.board.at(w, h).dc()->setSquarePos(sf::Vector2i(w, h), state);
			if (entity_t != object_type::NONE)
				addEntity(entity_t, sf::Vector2i(w, h));
		}
	}
	makeTexture();
}

template<typename T>
void Board<T>::makeTexture() {
	boardTexture = std::make_shared<sf::RenderTexture>();
	unsigned boardh = state.board.height(), boardw = state.board.width();
	unsigned width = (boardh + boardw) * sq::square_w / 2;
	unsigned height = (boardh + boardw) * sq::square_h / 2 + sq::square_t;
	if (!boardTexture->create(width, height))
	{
		std::cerr << "failed to create board texture";
	}

	boardTexture->clear(sf::Color::Black);

	for (auto i : state.board)i.dc()->draw(boardTexture.get());

	boardTexture->display();
	//boardView.setCenter(state.board.at(boardh / 2, boardw / 2).getSprite().getPosition());
}

struct selection {
	UnitComponent* u;
	pathsGrid g;
};

//template<typename T>
//selection Board<T>::select(UnitComponent* u)
//{
//	auto pathsGrid = getMoveRange(u, state);
//
//	unsigned start = entities.active();
//	unsigned end = start;
//	for (int i = ret.minX;i <= ret.maxX;i++) {
//		for (int j = ret.minY;j <= ret.maxY;j++) {
//			if (ret.grid.at(i, j).search == ret.search)
//				end++;
//			addEntity(object_type::MOVESELECT, sf::Vector2i(i, j));
//		}
//	}
//}