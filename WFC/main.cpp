#include <iostream>
#include <memory>
#include <SFML/Graphics.hpp>
#include <thread>
#include <chrono>
#include "FrameCounter.h"
#include "WFC.h"
#include "Board.h"
#include "TestBoards.h"
#include "PlayerState.h"
#include "Select.h"


auto getOutput(matrix<char>& in, unsigned owidth, unsigned oheight, unsigned n, bool rotate = true, bool reflect = true) {
	WFC<char> w(in, owidth, oheight, n, rotate, reflect);
	return w.run();
}
int main()
{
	sf::Clock clock;
	PlayerState p_state;
	auto output = getOutput(input, 20, 20, 3, true, true);

	sf::RenderWindow window(sf::VideoMode(1200, 800), "Dungeon Delve");

	auto tm = std::make_shared<ResourceManager<sf::Texture>>();
	Board<char> board(tm);
	auto& squares = board.state.board;
	window.setView(sf::View(sf::FloatRect(0.f, 0.f, (float)window.getSize().x / 2, (float)window.getSize().y / 2)));
	board.setSquares(output);
	window.setFramerateLimit(300);

	int lastX = -1, lastY = -1;
	FrameCounter fc;
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
				sf::FloatRect visibleArea(0.f, 0.f, event.size.width / 2, event.size.height / 2);
				window.setView(sf::View(visibleArea));
				break;
			}
			case sf::Event::MouseButtonPressed:
			{
				handleClick(board, window, clock.getElapsedTime(), p_state, event);
				break;
			}
			case sf::Event::MouseMoved:
			{
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
					//board.moveView(lastX - event.mouseMove.x, lastY - event.mouseMove.y);
					sf::View cview = window.getView();
					cview.move(lastX - event.mouseMove.x, lastY - event.mouseMove.y);
					window.setView(cview);
					fc.move(lastX - event.mouseMove.x, lastY - event.mouseMove.y);
				}
				lastX = event.mouseMove.x;
				lastY = event.mouseMove.y;
				break;
			}
			case sf::Event::KeyPressed:
			{
				std::cout << sf::Mouse::getPosition().x << " " << sf::Mouse::getPosition().y << std::endl;
				switch (event.key.code) {
				case sf::Keyboard::Key::A: {
					if (auto coords = board.getCoords(window, sf::Mouse::getPosition(window))) {
						auto& square = squares.at(coords.value());
						board.addEntity(rand() % 2 == 0 ? object_type::ROCK : object_type::CACTUS, coords.value());

					}
					break;
				}
				case sf::Keyboard::Key::U: {
					if (auto coords = board.getCoords(window, sf::Mouse::getPosition(window))) {
						auto& square = squares.at(coords.value());
						board.addEntity(rand() % 2 == 0 ? object_type::DUELIST : object_type::WOLF, coords.value());
					}
					break;
				}
				case sf::Keyboard::Key::D: {
					if (auto coords = board.getCoords(window, sf::Mouse::getPosition(window))) {
						auto& square = squares.at(coords.value());
						if (square.entities.size() != 0) {
							auto e = square.entities[0];
							board.removeEntity(e);
						}
						else {
							std::cout << "no entity!" << std::endl;
						}
					}
					break;
				}
				}
			}
			}
		}
		window.clear(sf::Color::Black);
		board.draw(window);
		fc.update(clock.getElapsedTime());
		fc.draw(window);
		window.display();
	}
	return 0;
}

