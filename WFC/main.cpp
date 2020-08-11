#include <SFML/Graphics.hpp>
#include <iostream>
class Tile
{
public:

private:
};

int main()
{
	int x = 150, y = 150;
	int step = 5;
	sf::RenderWindow window(sf::VideoMode(300, 300), "SFML works!");
	
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
				case sf::Event::KeyPressed:
					switch (event.key.code) {
						case sf::Keyboard::Up:
							if(y>=step) y -= step;
							break;
						case sf::Keyboard::Left:
							if (x >= step) x -= step;
							break;
						case sf::Keyboard::Down:
							if (300-y >= step) y += step;
							break;
						case sf::Keyboard::Right:
							if (300-x >= step) x += step;
							break;
					break;
				case sf::Event::LostFocus:
					std::cout << "Pausing." << std::endl;
					break;
				case sf::Event::GainedFocus:
					std::cout << "Resuming." << std::endl;
					break;
				case sf::Event::MouseMoved:
					//std::cout << event.mouseMove.x << " " << event.mouseMove.y << std::endl;
					break;
				case sf::Event::MouseButtonPressed:
					std::cout << event.mouseButton.button << std::endl;
					if (event.mouseButton.button == sf::Mouse::Right) {
						std::cout << event.mouseButton.x << " " << event.mouseButton.y << std::endl;
					}
					break;
			}
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear();
		window.draw(shape2);
		window.display();
	}

	return 0;
}