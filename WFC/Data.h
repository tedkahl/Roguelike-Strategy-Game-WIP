#pragma once
#include <map>
#include <string>
#include <SFML/Graphics.hpp>
namespace sq {
	const static float square_w = 80.f;
	const static float square_h = 40.f;
	const static float square_t = 15.f;
}
template<typename T>
struct Data {
public:
	std::map <T, std::tuple<std::string, std::string, sf::Vector2f>> squareinfo;
	std::map <T, std::tuple<std::string, std::string, sf::Vector2f>> entityinfo;
	Data();
};

template<typename T>
Data<T>::Data()
{
	squareinfo.emplace(std::make_pair('.', std::make_tuple("lava", "./textures/lava0.png", sf::Vector2f(0, 10))));
	squareinfo.emplace(std::make_pair(',', std::make_tuple("lava", "./textures/lava0.png", sf::Vector2f(0, 10))));
	squareinfo.emplace(std::make_pair('x', std::make_tuple("stone", "./textures/whitepaved0.png", sf::Vector2f(0, 0))));
	squareinfo.emplace(std::make_pair('C', std::make_tuple("stone", "./textures/whitepaved0.png", sf::Vector2f(0, 0))));
	squareinfo.emplace(std::make_pair('T', std::make_tuple("tallstone", "./textures/whitepavedtall.png", sf::Vector2f(0, -15))));
	squareinfo.emplace(std::make_pair('R', std::make_tuple("tallstone", "./textures/whitepavedtall.png", sf::Vector2f(0, -15))));

	entityinfo.emplace(std::make_pair('C', std::make_tuple("rock", "./textures/brownrock0.png", sf::Vector2f(8, -39))));
	entityinfo.emplace(std::make_pair('R', std::make_tuple("rock", "./textures/brownrock0.png", sf::Vector2f(8, -39))));
	entityinfo.emplace(std::make_pair(',', std::make_tuple("rock", "./textures/brownrock0.png", sf::Vector2f(8, -39))));
}