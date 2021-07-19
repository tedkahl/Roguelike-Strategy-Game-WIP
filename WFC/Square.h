#pragma once
#include <algorithm>
#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <SFML/Graphics.hpp>
#include <map>
#include "Drawable.h"

//having tm as a reference member prevented default constructor as reference must be initialized
template<typename T>
class Square
{
private:
	std::shared_ptr<ResourceManager<sf::Texture>> tm;
public:
	std::string name;
	T glyph;
	sf::Sprite sprite;
	Square() = default;
	Square(std::string n, const T glyph_, std::string path, ResourceManager<sf::Texture>& tm_);
	Square<T>& operator=(const Square<T>& other);
};
template<typename T>
Square<T>::Square(std::string n, const T glyph_, std::string path, ResourceManager<sf::Texture>& tm_) : tm(&tm_), name(n), glyph(glyph_), sprite() {
	sprite.setTexture(tm->get(path));
}

template<typename T>
Square<T>& Square<T>::operator=(const Square<T>& other)
{
	tm = other.tm;
	name = other.name;
	glyph = other.glyph;
	sprite = other.sprite;
	return *this;
}

template<typename T>
struct Board : protected Drawable
{
private:
	const static unsigned square_w = 80;
	const static unsigned square_h = 40;
	const static unsigned square_t = 15;
	const unsigned x_offset; //distance from the left corner of the board's display window to the left corner of the board
	const unsigned y_offset;
	std::map <T, std::pair<std::string, std::string>> squareinfo;
	std::shared_ptr<sf::RenderTexture> boardTexture;
	sf::View boardView;

	std::shared_ptr<ResourceManager<sf::Texture>> tm;
	void loadSquareInfo();
public:
	std::vector<std::vector<Square<T>>> board;
	Board(const float viewX, const float viewY, const float w, const float h, ResourceManager<sf::Texture>& tm_);
	void setSquares(std::vector<std::vector<T>>& WFCOutput);
	virtual void draw(sf::RenderWindow& window) override;

	std::vector<Square<T>>& operator[](size_t y);

};

template<typename T>
Board<T>::Board(const float viewX, const float viewY, const float w, const float h, ResourceManager<sf::Texture>& tm_) :Drawable("board", viewX, viewY, w, h), squareinfo(), x_offset(0), y_offset(0), tm(&tm_)
{
	loadSquareInfo();
}

template<typename T>
std::vector<Square<T>>& Board<T>::operator[](size_t y) {
	return board[y];
}

template<typename T>
void Board<T>::draw(sf::RenderWindow& window) {
	unsigned s_width = 80;
	unsigned s_height = 40;

	unsigned startX = x + x_offset;
	unsigned startY = y + y_offset;

	sf::Sprite boardSprite((*boardTexture).getTexture());
	boardSprite.setScale(2.0f, 2.0f);

	sf::View currView = window.getView();
	sf::View tempView;
	tempView.reset(sf::FloatRect(400, 400, 1000, 1000));
	//v.setViewport(sf::FloatRect(0.1f, 0.1f, 0.8f, 0.8f));

	window.setView(tempView);
	window.draw(boardSprite);
	window.setView(currView);
}

template<typename T>
void Board<T>::loadSquareInfo() {
	squareinfo.emplace(std::make_pair('.', std::make_pair("lava", "./textures/lava0.png")));
	squareinfo.emplace(std::make_pair('x', std::make_pair("stone", "./textures/whitepaved0.png")));
}

template<typename T>
void Board<T>::setSquares(std::vector<std::vector<T>>& WFCOutput)
{
	board = std::move(std::vector<std::vector<Square<T>>>(WFCOutput.size(), std::vector<Square<T>>(WFCOutput[0].size())));

	for (unsigned i = 0;i < WFCOutput.size();i++) {
		for (unsigned j = 0;j < WFCOutput[0].size();j++) {
			auto [name, path] = squareinfo.at(WFCOutput[i][j]);

			board[i][j] = Square<T>(name, WFCOutput[i][j], path, *tm);
		}
	}

	boardTexture = std::make_shared<sf::RenderTexture>();
	unsigned boardw = board[0].size(), boardh = board.size();
	unsigned width = (boardh + boardw) * square_w / 2;
	unsigned height = (boardh + boardw) * square_h / 2 + square_t;
	if (!boardTexture->create(width, height))
	{
		std::cerr << "failed to create board texture";
	}
	boardTexture->clear(sf::Color::Black);
	int lowerY;
	unsigned x = 0, y = boardw * square_h / 2;
	for (int i = boardh - 1;i >= 0;i--) {
		for (int j = 0;j < boardw;j++) {
			lowerY = (WFCOutput[i][j] == '.') ? 10 : 0;
			board[i][j].sprite.setPosition((i + j) * square_w / 2, (boardh - 1 - i + j) * square_h / 2 + lowerY);
			boardTexture->draw(board[i][j].sprite);
		}
	}
	boardTexture->display();
}

