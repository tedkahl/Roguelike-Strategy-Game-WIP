#include <iostream>
#include <SFML/Graphics.hpp>
#include <thread>
#include <chrono>
#include "WFC.h"
#include "Board.h"
#include "Data.h"
int main()
{

	matrix<char> input0(32, 16,
		{ '.','.','.','x','.','.','.','.','.','.','.','.','x','.','.','.',
		  '.','.','.','x','.','.','.','.','.','x','x','x','x','x','x','.',
		  '.','x','x','x','x','x','.','.','.','x','x','x','x','x','x','.',
		  '.','x','x','x','x','x','.','.','.','x','x','x','x','x','x','.',
		  '.','x','x','x','x','x','x','x','x','x','x','x','x','x','x','.',
		  'x','x','x','x','x','x','.','.','.','x','x','x','x','x','x','x',
		  '.','x','x','x','x','x','.','.','.','x','x','x','x','x','x','.',
		  '.','.','x','.','.','.','.','.','.','x','x','x','x','x','x','.',
		  '.','.','x','.','.','.','.','.','.','.','.','x','.','.','.','.',
		  '.','.','x','.','.','.','.','.','.','.','.','x','.','.','.','.',
		  '.','.','x','x','x','x','x','x','.','.','x','x','x','x','.','.',
		  'x','x','x','x','x','x','x','x','.','.','x','x','x','x','x','x',
		  '.','.','x','x','x','x','x','x','x','x','x','x','x','x','.','.',
		  '.','.','x','x','x','x','x','x','.','.','x','x','x','x','.','.',
		  '.','.','x','x','x','x','x','x','.','.','.','.','x','.','.','.',
		  '.','.','.','x','.','.','.','.','.','.','.','.','x','.','.','.',
		  '.','.','.','x','.','.','.','.','.','.','.','.','x','.','.','.',
		  '.','.','.','x','.','.','.','.','.','x','x','x','x','x','x','.',
		  '.','x','x','x','C','x','.','.','.','x','x','x','x','x','x','.',
		  '.','x','x','x','x','x','.','.','.','x','x','x','x','C','x','.',
		  '.','x','x','x','x','x','x','x','x','x','x','x','C','C','x','.',
		  'x','x','x','x','x','x','.','.','.','x','x','x','x','x','x','x',
		  '.','x','x','x','x','x','.','.','.','x','C','x','x','x','x','.',
		  '.','.','x','.','.','.','.','C','.','x','x','x','x','x','x','.',
		  '.','.','x','.','.','.','.','.','.','.','.','x','.','.','.','.',
		  '.','.','x','.','.','.','.','.','.','.','.','x','.','.','.','.',
		  '.','.','x','x','x','x','x','x','.','.','x','x','x','x','.','.',
		  'x','x','x','x','x','x','x','x','.','.','x','x','x','x','x','x',
		  '.','.','C','C','x','C','x','x','x','x','x','x','x','x','.','.',
		  '.','.','x','x','x','x','x','x','.','.','x','x','x','x','.','.',
		  '.','.','x','x','x','x','x','x','.','.','.','.','x','.','.','.',
		  '.','.','.','x','.','.','.','.','.','.','.','.','x','.','.','.' });



	matrix<char> input(16, 16,
		{ '.','.','.','x','.','.','.','.','.','.','.','.','x','.','.','.',
		  '.','.','.','x','.','.','.','.','.','x','x','x','x','x','x','.',
		  '.','x','x','x','x','x','.','.','.','x','x','x','x','x','x','.',
		  '.','x','x','x','x','x','.','.','.','x','x','x','x','x','x','.',
		  '.','x','x','x','x','x','x','x','x','x','x','x','x','x','x','.',
		  'x','x','x','x','x','x','.','.','.','x','x','x','x','x','x','x',
		  '.','x','x','x','x','x','.','.','.','x','x','x','x','x','x','.',
		  '.','.','x','.','.','.','.','.','.','x','x','x','x','x','x','.',
		  '.','.','x','.','.','.','.','.','.','.','.','x','.','.','.','.',
		  '.','.','x','.','.','.','.','.','.','.','.','x','.','.','.','.',
		  '.','.','x','x','x','x','x','x','.','.','x','x','x','x','.','.',
		  'x','x','x','x','x','x','x','x','.','.','x','x','x','x','x','x',
		  '.','.','x','x','x','x','x','x','x','x','x','x','x','x','.','.',
		  '.','.','x','x','x','x','x','x','.','.','x','x','x','x','.','.',
		  '.','.','x','x','x','x','x','x','.','.','.','.','x','.','.','.',
		  '.','.','.','x','.','.','.','.','.','.','.','.','x','.','.','.' });

	WFC<char> w(input0, 20, 20, 3, true, true);
	auto output = w.run();

	sf::RenderWindow window(sf::VideoMode(1200, 800), "Dungeon Delve");

	ResourceManager<sf::Texture> t;
	Data<char> d;
	Board<char> board(t, d);
	board.resizeView(window.getDefaultView().getSize().x, window.getDefaultView().getSize().y);
	board.setSquares(output);
	window.setFramerateLimit(60);
	std::vector<BoardEntity> entities;


	int lastX = -1, lastY = -1;
	while (window.isOpen())
	{
		sf::Event event;


		while (window.pollEvent(event))
		{
			switch (event.type) {
			case sf::Event::Closed:
			{
				window.close();
				break;
			}
			case sf::Event::Resized:
			{
				// update the view to the new size of the window
				sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
				window.setView(sf::View(visibleArea));
				board.resizeView(event.size.width, event.size.height);
				break;
			}
			case sf::Event::MouseButtonPressed:
			{
				std::cout << "Pixel:" << event.mouseButton.x << " " << event.mouseButton.y << std::endl;
				std::cout << "View:" << window.getView().getCenter().x << " " << window.getView().getCenter().y << " " << window.getView().getSize().x << " " << window.getView().getSize().y << std::endl;
				board.getSquare(window, sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
				break;
			}
			case sf::Event::MouseMoved:
			{
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
					board.moveView(lastX - event.mouseMove.x, lastY - event.mouseMove.y);
					/*auto cview = window.getView();
					cview.move(lastX - event.mouseMove.x, lastY - event.mouseMove.y);
					window.setView(cview);*/
				}
				lastX = event.mouseMove.x;
				lastY = event.mouseMove.y;
				break;
			}
			}
		}
		window.clear(sf::Color::Black);
		board.draw(window);
		window.display();
	}
	return 0;
}