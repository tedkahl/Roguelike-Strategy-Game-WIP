#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
struct GameState {
	int active_player;
	sf::Time last_AI_move;
	sf::Time now;
	const sf::Time AIMoveInterval = sf::milliseconds(250);
	GameState() :active_player(-1), last_AI_move(), now() {}
};