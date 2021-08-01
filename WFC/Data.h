#pragma once
#include <map>
#include <string>
#include <SFML/Graphics.hpp>
namespace sq {
	const static float square_w = 80.f;
	const static float square_h = 40.f;
	const static float square_t = 15.f;
}
enum terrain_type {
	GRASS, WATER, PAVEDSTONE, PAVEDSTONE_TALL, ROUGHSTONE, LAVA, SAND
};

enum object_type {
	ROCK
};
template<typename T>
struct Data {
public:
	std::map <T, std::tuple<terrain_type, std::string, sf::Vector2f>> squareinfo;
	std::map <T, std::tuple<object_type, std::string, sf::Vector2f>> entityinfo;
	Data();
};

template<typename T>
Data<T>::Data()
{
	squareinfo.emplace(std::make_pair('.', std::make_tuple(terrain_type::LAVA, "./textures/lava0.png", sf::Vector2f(0, 10))));
	squareinfo.emplace(std::make_pair('x', std::make_tuple(terrain_type::PAVEDSTONE, "./textures/whitepaved0.png", sf::Vector2f(0, 0))));
	squareinfo.emplace(std::make_pair('C', std::make_tuple(terrain_type::PAVEDSTONE, "./textures/whitepaved0.png", sf::Vector2f(0, 0))));
	squareinfo.emplace(std::make_pair('T', std::make_tuple(terrain_type::PAVEDSTONE_TALL, "./textures/whitepavedtall.png", sf::Vector2f(0, -15))));
	squareinfo.emplace(std::make_pair('R', std::make_tuple(terrain_type::PAVEDSTONE_TALL, "./textures/whitepavedtall.png", sf::Vector2f(0, -15))));

	entityinfo.emplace(std::make_pair('C', std::make_tuple(object_type::ROCK, "./textures/brownrock0.png", sf::Vector2f(9, -40))));
	entityinfo.emplace(std::make_pair('R', std::make_tuple(object_type::ROCK, "./textures/brownrock0.png", sf::Vector2f(9, -40))));

	squareinfo.emplace(std::make_pair('w', std::make_tuple(terrain_type::WATER, "./textures/lightwater0.png", sf::Vector2f(0, 0))));
	squareinfo.emplace(std::make_pair('g', std::make_tuple(terrain_type::GRASS, "./textures/grass0.png", sf::Vector2f(0, 0))));
	squareinfo.emplace(std::make_pair('s', std::make_tuple(terrain_type::SAND, "./textures/sand0.png", sf::Vector2f(0, 0))));
}