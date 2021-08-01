#pragma once
#include <set>
#include "Square.h"
#include "DataManager.h"
#include "matrix.h"
#include "BoardState.h"
template<typename T>
class Board
{
private:
	std::shared_ptr<sf::RenderTexture> boardTexture;
	std::shared_ptr<ResourceManager<sf::Texture>> tm_;
	std::shared_ptr<Data<T>> data;
	DataManager<BoardEntity> entities;
	DataManager<DrawComponent> dcomponents;
	BoardEntity* selected;
	//sf::View boardView;

	void makeTexture();
public:
	BoardState state;
	Board(std::shared_ptr<ResourceManager<sf::Texture>> tm, Data<T>& data_);
	bool addEntity(T glyph, std::pair<unsigned, unsigned> coords);
	bool removeEntity(BoardEntity* e);
	void setEntityPos(BoardEntity* e, std::pair<unsigned, unsigned> newpos);
	void setSquares(matrix<T>& WFCOutput);
	void draw(sf::RenderWindow& window);
	//inline void moveView(float offsetX, float offsetY) { boardView.move(offsetX, offsetY); };
	//inline void resizeView(float width, float height) { boardView.setSize(width / 2, height / 2); };
	std::optional < std::pair<unsigned, unsigned >> getCoords(sf::RenderWindow& window, sf::Vector2i pixel);
};

template<typename T>
Board<T>::Board(std::shared_ptr<ResourceManager<sf::Texture>> tm, Data<T>& data_) :tm_(tm), data(&data_) {}

template<typename T>
void Board<T>::setEntityPos(BoardEntity* e, std::pair<unsigned, unsigned> newpos) {
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
std::optional<std::pair<unsigned, unsigned>> Board<T>::getCoords(sf::RenderWindow& window, sf::Vector2i pixel) {
	auto coords = window.mapPixelToCoords(pixel);

	float x_pos = coords.y + (.5 * coords.x);
	float y_pos = coords.y - (.5 * coords.x);
	x_pos = x_pos / sq::square_h - (float)state.board.height() / 2;
	y_pos = y_pos / sq::square_h + (float)state.board.height() / 2;

	std::cout << "xy: " << x_pos << " " << y_pos << std::endl;
	if (0 <= x_pos && x_pos < state.board.width() && 0 <= y_pos && y_pos < state.board.height())
		return std::make_pair((unsigned)x_pos, (unsigned)y_pos);
	return std::nullopt;
}


template<typename T>
void Board<T>::draw(sf::RenderWindow& window) {

	/*for (auto i : state.board) {
		i.dc()->draw(&window);
		for (auto j : i.entities) {
			j->dc()->draw(&window);
		}
	}*/
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
bool Board<T>::addEntity(T glyph, std::pair<unsigned, unsigned> coords)
{
	std::cout << state.board.at(coords).entities.size();
	if (state.board.at(coords).entities.size() != 0) {
		std::cout << "entity already here";
		return false;
	}
	auto search = (data->entityinfo).find(glyph);
	if (search != data->entityinfo.end()) {
		auto [ename, epath, eoffset] = search->second;

		auto dc = dcomponents.declareNew(epath, eoffset, tm_, 5);
		auto entity = entities.declareNew(ename, dc);
		entity->setPos(coords, state);
		assert(entity->getOwner() == coords);
		assert(entity->dc() == dc);
		assert(std::get<BoardEntity*>(dc->getOwner()) == entity);
		state.board.at(coords).addE(entity);
		return true;
	}
	return false;
}

template<typename T>
bool Board<T>::removeEntity(BoardEntity* e)
{
	std::cout << "e " << e << " index " << e->index() << std::endl;
	std::cout << "coords " << e->getOwner().first << " " << e->getOwner().second << std::endl;
	int index = e->index();
	if (state.board.at(e->getPos()).removeE(e)) {
		auto [parent, new_dc_ptr] = dcomponents.deactivate(e->dc()->index());
		std::visit([&](auto ptr) {ptr->setDC(new_dc_ptr);}, parent);
		/*if (std::holds_alternative<BoardEntity*>(parent)) {
			std::get<BoardEntity*>(parent)->setDC(new_dc_ptr);
		}
		else {
			std::get<Square*>(parent)->setDC(new_dc_ptr);
		}*/
		auto [square, new_e_ptr] = entities.deactivate(index);
		if (entities.firstInvalidPtr() != e)
			state.board.at(square).replaceE(entities.firstInvalidPtr(), new_e_ptr);
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
			auto [name, path, offset] = data->squareinfo.at(val);
			state.board.at(w, h) = Square(name, dcomponents.declareNew(path, offset, tm_, 0));
			state.board.at(w, h).dc()->setSquarePos(std::make_pair(w, h), state);
			addEntity(val, std::make_pair(w, h));
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

void select(Unit* e, BoardState& state)
{

}