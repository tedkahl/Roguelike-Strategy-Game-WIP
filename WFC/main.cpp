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
		  '.','.','x','x','x','x','x','x','.','.','T','T','T','T','.','.',
		  'x','x','x','x','x','x','x','x','.','.','T','T','T','T','x','x',
		  '.','.','x','x','R','R','x','x','x','x','T','T','T','T','.','.',
		  '.','.','x','x','x','x','x','x','.','.','T','T','T','T','.','.',
		  '.','.','x','x','x','x','x','x','.','.','.','.','x','.','.','.',
		  '.','.','.','x','.','.','.','.','.','.','.','.','x','.','.','.',
		  '.','.','.','x','.','.','.','.','.','.','.','.','x','.','.','.',
		  '.','.','.','x','.','.','.','.','.','x','x','x','x','x','x','.',
		  '.','x','x','x','C','x','.','.','.','x','x','x','x','x','x','.',
		  '.','x','x','x','x','x','.','.','.','x','x','x','x','C','x','.',
		  '.','x','x','x','x','x','x','x','x','x','x','x','C','C','x','.',
		  'x','x','x','x','x','x','.','.','.','x','x','x','C','C','x','x',
		  '.','x','x','x','x','x','.','.','.','x','C','x','x','x','x','.',
		  '.','.','x','.','.','.','.',',','.','x','x','x','x','x','x','.',
		  '.','.','x','.','.','.','.','.','.','.','.','x','.','.','.','.',
		  '.','.','x','.','.','.','.','.','.','.','.','x','.','.','.','.',
		  '.','.','x','x','x','x','x','x','.','.','x','x','x','x','.','.',
		  'x','x','x','x','x','x','x','x','.','.','x','x','x','x','x','x',
		  '.','.','C','C','x','R','x','x','x','x','x','x','x','x','.','.',
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

	matrix<char> input1(3, 3,
		{ 'T','T','T',
		  '.','.','.',
		  '.','.','.' });

	WFC<char> w(input0, 20, 20, 3, true, true);
	auto output = w.run();

	sf::RenderWindow window(sf::VideoMode(1200, 800), "Dungeon Delve");

	auto tm = std::make_shared<ResourceManager<sf::Texture>>();
	Data<char> d;
	Board<char> board(tm, d);
	auto& squares = board.state.board;
	//board.resizeView(window.getDefaultView().getSize().x, window.getDefaultView().getSize().y);
	window.setView(sf::View(sf::FloatRect(0, 0, window.getSize().x / 2, window.getSize().y / 2)));
	board.setSquares(output);
	window.setFramerateLimit(400);


	int lastX = -1, lastY = -1;
	int frames = 0;
	sf::Clock clock;
	sf::Time t;
	sf::Font roboto;
	roboto.loadFromFile("./Roboto/Roboto-Regular.ttf");
	sf::Text ftext("60", roboto, 16);
	ftext.setPosition(0, 0);
	ftext.setFillColor(sf::Color::White);
	ftext.setOutlineColor(sf::Color::Black);
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
				sf::FloatRect visibleArea(0, 0, event.size.width / 2, event.size.height / 2);
				window.setView(sf::View(visibleArea));
				//board.resizeView(event.size.width, event.size.height);
				break;
			}
			case sf::Event::MouseButtonPressed:
			{
				/*std::cout << "Pixel:" << event.mouseButton.x << " " << event.mouseButton.y << std::endl;
				std::cout << "View:" << window.getView().getCenter().x << " " << window.getView().getCenter().y << " " << window.getView().getSize().x << " " << window.getView().getSize().y << std::endl;*/
				board.getCoords(window, sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
				break;
			}
			case sf::Event::MouseMoved:
			{
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
					//board.moveView(lastX - event.mouseMove.x, lastY - event.mouseMove.y);
					sf::View cview = window.getView();
					cview.move(lastX - event.mouseMove.x, lastY - event.mouseMove.y);
					window.setView(cview);
					ftext.move(lastX - event.mouseMove.x, lastY - event.mouseMove.y);
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
						if (square.entities.size() == 0) {
							board.addEntity('C', coords.value());
						}
					}
					break;
				}
				case sf::Keyboard::Key::D: {
					if (auto coords = board.getCoords(window, sf::Mouse::getPosition(window))) {
						auto& square = squares.at(coords.value());
						if (square.entities.size() > 0) {
							auto e = square.entities[0];
							board.removeEntity(e);
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
		frames++;
		sf::Time t2 = clock.getElapsedTime();
		if (t2 - t > sf::seconds(.5)) {
			ftext.setString(std::to_string(frames * 2));
			t = t2;
			frames = 0;
		}
		window.draw(ftext);
		window.display();
	}
	return 0;
}