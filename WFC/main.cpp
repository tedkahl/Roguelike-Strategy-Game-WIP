#include <iostream>
#include <memory>
#include <SFML/Graphics.hpp>
#include <thread>
#include <chrono>
#include "FrameCounter.h"
#include "WFC.h"
#include "Level.h"
#include "TestBoards.h"
#include "PlayerState.h"
#include "Select.h"
#include "AI.h"

auto getOutput(matrix<char>& in, unsigned owidth, unsigned oheight, unsigned n, bool rotate = true, bool reflect = true) {
	WFC<char> w(in, owidth, oheight, n, rotate, reflect);
	return w.run();
}
int main()
{
	sf::Clock clock;
	PlayerState p_state(0);
	auto output = getOutput(input0, 20, 20, 3, true, true);

	sf::RenderWindow window(sf::VideoMode(1200, 800), "Dungeon Delve");

	auto tm = std::make_shared<ResourceManager<sf::Texture>>();
	Level level(tm);
	auto& squares = level.state.board;
	window.setView(sf::View(sf::FloatRect(0.f, 0.f, (float)window.getSize().x / 2, (float)window.getSize().y / 2)));
	level.setSquares(output);
	window.setFramerateLimit(30);

	float lastX = -1., lastY = -1.;
	FrameCounter fc;

	//will be smarter than this eventually
	AIPlayer main_enemy(level, 1);
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (p_state.getNext() == nullptr) {
				main_enemy.startTurn(clock.getElapsedTime());
				//AI player turn starts
			}
			switch (event.type) {
			case sf::Event::Closed:
			{
				window.close();
				break;
			}
			case sf::Event::Resized:
			{
				// update the view to the new size of the window
				sf::FloatRect visibleArea(0.f, 0.f, static_cast<float>(event.size.width) / 2, static_cast<float>(event.size.height) / 2);
				window.setView(sf::View(visibleArea));
				break;
			}
			case sf::Event::KeyPressed:
			case sf::Event::MouseButtonPressed:
			{
				handleInput(level, window, clock.getElapsedTime(), p_state, event);
				break;
			}
			case sf::Event::MouseMoved:
			{
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
					//level.moveView(lastX - event.mouseMove.x, lastY - event.mouseMove.y);
					sf::View cview = window.getView();
					cview.move(lastX - event.mouseMove.x, lastY - event.mouseMove.y);
					window.setView(cview);
					fc.move(lastX - event.mouseMove.x, lastY - event.mouseMove.y);
				}
				lastX = static_cast<float>(event.mouseMove.x);
				lastY = static_cast<float>(event.mouseMove.y);
				break;
			}
			}
			window.clear(sf::Color::Black);
			level.update(clock.getElapsedTime());
			level.draw(window);
			fc.update(clock.getElapsedTime());
			fc.draw(window);
			window.display();
		}
		return 0;
	}
}
