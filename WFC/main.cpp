﻿#include <SFML/Graphics.hpp>
#include <iostream>
#include "WFC.h"

int main()
{
	/*sf::RenderWindow window(sf::VideoMode(300, 300), "SFML works!");

	sf::RectangleShape shape2({ 10,10 });

	shape2.setFillColor(sf::Color::Red);
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch(event.type){
				case sf::Event::Closed:
					window.close();
					break;
			}
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear();
		window.draw(shape2);
		window.display();
	}*/
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
	  {'x','x','x','x','x','x','x','x','.','.','x','x','x','x','.','.'},
	  {'.','.','x','x','x','x','x','x','x','x','x','x','x','x','.','.'},
	  {'.','.','x','x','x','x','x','x','.','.','x','x','x','x','.','.'},
	  {'.','.','x','x','x','x','x','x','.','.','.','.','x','.','.','.'},
	  {'.','.','.','x','.','.','.','.','.','.','.','.','x','.','.','.'},
	};
	WFC<char> (input, 40, 40, 3);
	return 0;
}