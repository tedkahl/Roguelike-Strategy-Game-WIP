#include <SFML/Graphics.hpp>
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
	std::vector<std::vector<int>> input = { { 1,1,1 }, { 2,2,2 }, { 1,1,2 } };
	WFC<int> w(input, 6, 6, 2);
	return 0;
}