#pragma once
#include <SFML/Graphics.hpp>
#include "ResourceManager.h"
class FrameCounter {
private:
	int frames = 0;
	sf::Text ftext;
	sf::Time prev;
public:
	FrameCounter(sf::Font& font) {
		ftext = sf::Text("60", font, 16);
		ftext.setPosition(0, 0);
		ftext.setFillColor(sf::Color::White);
		ftext.setOutlineColor(sf::Color::Black);
	}
	void update(const sf::Time& curr) {
		frames++;
		if (curr - prev > sf::seconds(.5)) {
			ftext.setString(std::to_string(frames * 2));
			prev = curr;
			frames = 0;
		}
	}
	void move(float x, float y) {
		ftext.move(x, y);
	}
	void draw(sf::RenderWindow& window) {
		window.draw(ftext);
	}
};