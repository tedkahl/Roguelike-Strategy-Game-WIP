#include <iostream>
#include <SFML/Graphics.hpp>
#include "Window.h"
#include "WFC.h"
#include "Square.h"
int main()
{

	/*std::vector<std::vector<char>> input =
	{ {'.','.','.','.'},
	  {'.','x','x','x'},
	  {'.','x','R','x'},
	  {'.','x','x','x'}

	};*/



	std::vector<std::vector<char>> input =
	{ {'.','.','.','x','.','.','.','.','.','.','.','.','x','.','.','.'},
	  {'.','.','.','x','.','.','.','.','.','x','x','x','x','x','x','.'},
	  {'.','x','x','x','x','x','.','.','.','x','x','x','x','x','x','.'},
	  {'.','x','x','x','x','x','.','.','.','x','x','x','x','x','x','.'},
	  {'.','x','x','x','x','x','x','x','x','x','x','x','x','x','x','.'},
	  {'x','x','x','x','x','x','.','.','.','x','x','x','x','x','x','x'},
	  {'.','x','x','x','x','x','.','.','.','x','x','x','x','x','x','.'},
	  {'.','.','x','.','.','.','.','.','.','x','x','x','x','x','x','.'},
	  {'.','.','x','.','.','.','.','.','.','.','.','x','.','.','.','.'},
	  {'.','.','x','.','.','.','.','.','.','.','.','x','.','.','.','.'},
	  {'.','.','x','x','x','x','x','x','.','.','x','x','x','x','.','.'},
	  {'x','x','x','x','x','x','x','x','.','.','x','x','x','x','x','x'},
	  {'.','.','x','x','x','x','x','x','x','x','x','x','x','x','.','.'},
	  {'.','.','x','x','x','x','x','x','.','.','x','x','x','x','.','.'},
	  {'.','.','x','x','x','x','x','x','.','.','.','.','x','.','.','.'},
	  {'.','.','.','x','.','.','.','.','.','.','.','.','x','.','.','.'} };
	//WFC<char> w(input, 6, 6, 3, true, true);
	//auto output = w.run();

	sf::RenderWindow window(sf::VideoMode(1200, 800), "Dungeon Delve");

	ResourceManager<sf::Texture> t;
	Board<char> board(20, 20, 600, 600, t);
	board.setSquares(input);
	window.setFramerateLimit(60);
	while (window.isOpen())
	{
		sf::Event event;


		while (window.pollEvent(event))
		{
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::Resized:
				// update the view to the new size of the window
				std::cout << event.size.width <<" "<< event.size.height<< std::endl;
				sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
				window.setView(sf::View(visibleArea));
				break;
			}
		}
		window.clear(sf::Color::Black);
		board.draw(window);
		window.display();
	}
	return 0;
}