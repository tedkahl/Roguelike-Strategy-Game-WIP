#pragma once
#include "Square.h"
template<typename T>
class Board
{
private:
	std::shared_ptr<sf::RenderTexture> boardTexture;
	std::shared_ptr<ResourceManager<sf::Texture>> tm;
	std::shared_ptr<Data<T>> data;
	std::vector < BoardEntity> entities;
	sf::View boardView;

	void makeTexture();
	void loadInfo();
public:
	matrix<Square> board;
	Board(ResourceManager<sf::Texture>& tm_, Data<T>& data_);
	void setEntityPos(BoardEntity* e, std::pair<unsigned, unsigned> newpos);
	void setSquares(matrix<T>& WFCOutput);
	void draw(sf::RenderWindow& window);
	inline void moveView(float offsetX, float offsetY) { boardView.move(offsetX, offsetY); };
	inline void resizeView(float width, float height) { boardView.setSize(width / 2, height / 2); };
	Square& getSquare(sf::RenderWindow& window, sf::Vector2i pixel);
};

template<typename T>
Board<T>::Board(ResourceManager<sf::Texture>& tm_, Data<T>& data_) :tm(&tm_), data(&data_) {}

template<typename T>
void Board<T>::setEntityPos(BoardEntity* e, std::pair<unsigned, unsigned> newpos) {
	auto coords = e->getPos();
	assert(board.at(coords).removeEntity(e));
	board.at(newpos).addE(e);
	e->setPos(newpos);
	e->updateSpritePos();
}

template<typename T>
Square& Board<T>::getSquare(sf::RenderWindow& window, sf::Vector2i pixel) {
	sf::View currView = window.getView();
	window.setView(boardView);
	auto coords = window.mapPixelToCoords(pixel);
	window.setView(currView);
	//pixel + boardView.getCenter() - (boardView.getSize() / 2.0f);

	float x_pos = coords.y + (.5 * coords.x);
	float y_pos = coords.y - (.5 * coords.x);
	x_pos = x_pos / 80 - (float)board.height() / 2;
	y_pos = (float)board.height() / 2 - y_pos / 80;

	std::cout << "xy: " << x_pos << " " << y_pos << std::endl;
	return board.at((unsigned)x_pos, (unsigned)y_pos);
}


template<typename T>
void Board<T>::draw(sf::RenderWindow& window) {
	sf::Sprite boardSprite((*boardTexture).getTexture());

	sf::View currView = window.getView();

	window.setView(boardView);
	window.draw(boardSprite);
	for (auto i : entities) i.draw(&window);
	window.setView(currView);
}

template<typename T>
void Board<T>::loadInfo() {
}

template<typename T>
void Board<T>::setSquares(matrix<T>& WFCOutput)
{
	board.set(WFCOutput.width(), WFCOutput.height(), Square());

	for (unsigned i = 0;i < WFCOutput.width();i++) {
		for (unsigned j = 0;j < WFCOutput.height();j++) {
			T val = WFCOutput.at(i, j);
			auto [name, path, offset] = data->squareinfo.at(val);
			board.at(i, j) = Square(name, path, offset, *tm);
			board.at(i, j).setSpritePos(sf::Vector2f((i + j) * sq::square_w / 2, (board.height() - 1 + i - j) * sq::square_h / 2));

			auto search = (data->entityinfo).find(val);
			if (search != data->entityinfo.end()) {
				auto [ename, epath, eoffset] = search->second;

				entities.push_back(BoardEntity(ename, epath, eoffset, *tm));
				auto e = std::make_shared<BoardEntity>(entities.back());
				e->setPos(std::make_pair(i, j));
				board.at(i, j).addE(e);
				e->updateSpritePos();
			}
		}
	}
	makeTexture();
}

template<typename T>
void Board<T>::makeTexture() {
	boardTexture = std::make_shared<sf::RenderTexture>();
	unsigned boardh = board.height(), boardw = board.width();
	unsigned width = (boardh + boardw) * sq::square_w / 2;
	unsigned height = (boardh + boardw) * sq::square_h / 2 + sq::square_t;
	if (!boardTexture->create(width, height))
	{
		std::cerr << "failed to create board texture";
	}

	boardTexture->clear(sf::Color::Black);

	for (int i = boardh - 1; i >= 0;i--) {
		for (int j = 0;j < boardw;j++) {
			board.at(j, i).draw(boardTexture.get());
		}
	}
	boardTexture->display();
	boardView.setCenter(board.at(boardh / 2, boardw / 2).getSprite().getPosition());
}
