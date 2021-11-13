#include <iostream>
#include <memory>
#include <SFML/Graphics.hpp>
#include <thread>
#include <chrono>
#include <ranges>
#include "FrameCounter.h"
#include "Team.h"
#include "WFC.h"
#include "Level.h"
#include "TestBoards.h"
#include "Player.h"
#include "Select.h"
#include "AI.h"

auto getOutput(matrix<char>& in, unsigned owidth, unsigned oheight, unsigned n, bool rotate = true, bool reflect = true) {
	WFC<char> w(in, owidth, oheight, n, rotate, reflect);
	return w.run();
}

int main()
{
	sf::Clock clock;
	auto output = getOutput(input, 20, 20, 3, true, true);

	sf::RenderWindow window(sf::VideoMode(1200, 800), "Dungeon Delve");

	auto tm = std::make_shared<ResourceManager<sf::Texture>>();
	Level level(tm);
	Player p_state(level, 0);
	cout << "level " << sizeof(level);
	auto& squares = level.state.board;
	window.setView(sf::View(sf::FloatRect(0.f, 0.f, (float)window.getSize().x / 2, (float)window.getSize().y / 2)));
	level.setSquares(output);
	window.setFramerateLimit(300);

	float lastX = -1., lastY = -1.;
	FrameCounter fc;
	GameState g;
	//AI stuff, placeholder
	AIPlayer main_enemy(level, 1);
	cout << "ai player " << sizeof(AIPlayer);
	bool blocked = false;
	for (int i = 0;i < 10;i++) {
		for (int j = 0;j < 10;j++) {
			level.addEntityTest(object_type::DUELIST, 0, { i,j });
		}
	}
	while (window.isOpen())
	{
		g.now = clock.getElapsedTime();
		//AI loop. Hideous, improve
		if (g.active_player == 0 && p_state.is_done()) {
			cout << "switching active player" << endl;
			g.active_player = ++g.active_player % (Alliance::instance()->numTeams() - 1);
			main_enemy.startTurn();
			g.last_AI_move = g.now - g.AIMoveInterval;
			//AI player turn starts
		}
		else if (g.active_player == 1 && g.now - g.last_AI_move >= g.AIMoveInterval && !blocked) {
			if (!main_enemy.is_done()) {
				if (main_enemy.hasSelection())
					main_enemy.executeSelected(g.now);
				else
					main_enemy.selectNext();
				g.last_AI_move = g.now;
			}
			else {
				g.active_player = ++g.active_player % (Alliance::instance()->numTeams() - 1);
				cout << "switching active player" << endl;
				p_state.startTurn();
			}
		}
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
				sf::FloatRect visibleArea(0.f, 0.f, static_cast<float>(event.size.width) / 2, static_cast<float>(event.size.height) / 2);
				window.setView(sf::View(visibleArea));
				break;
			}
			case sf::Event::KeyPressed:
			case sf::Event::MouseButtonPressed:
			{
				handleInput(level, window, g, p_state, main_enemy, event);
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
		}
		window.clear(sf::Color::Black);
		blocked = level.update(g.now);
		level.draw(window);
		fc.update(g.now);
		fc.draw(window);
		window.display();
	}
	return 0;
}
