#pragma once
#include <map>
#include <memory>
#include <string>
#include <SFML/Graphics.hpp>
#include "type_enums.h"
namespace sq {
	const static float square_w = 80.f;
	const static float square_h = 40.f;
	const static float square_t = 15.f;
}
template<typename T>
struct Data;

template<typename T>
struct Data {
private:
	Data();
	inline static std::unique_ptr<Data<T>> instance_;
public:
	static Data<T>* d() {
		if (!instance_) instance_ = std::unique_ptr<Data<T>>(new Data<T>());
		return instance_.get();
	}
	~Data() = default;
	Data(const Data<T>&) = delete;
	Data& operator= (const Data<T>) = delete;
	std::map < T, std::tuple<terrain_type, object_type>> glyphs;
	std::map <unsigned, std::tuple<std::string, sf::Vector2f, sf::IntRect>> squareinfo; //path, offset, texture rect
	std::map <unsigned, std::tuple<std::string, sf::Vector2f, sf::IntRect>> entityinfo;
	std::array<std::array<int, terrain_type::TERRAIN_END>, move_type::MOVE_END> movecosts;
};

template<typename T>
void combine_vectors(std::vector<T>& first, std::vector<std::vector<T>>&& vecs) {
	for (auto&& i : vecs) {
		first.reserve(first.size() + i.size());
		first.insert(first.end(), std::make_move_iterator(i.begin()), std::make_move_iterator(i.end()));
	}
}

template<typename T>
Data<T>::Data()
{
	movecosts[move_type::FLY].fill(1);
	movecosts[move_type::WALK] = { 1,2,1,1,1,999,2 };

	glyphs.emplace(std::make_pair('.', std::make_tuple(terrain_type::LAVA, object_type::NONE)));
	glyphs.emplace(std::make_pair('x', std::make_tuple(terrain_type::PAVEDSTONE, object_type::NONE)));
	glyphs.emplace(std::make_pair('C', std::make_tuple(terrain_type::PAVEDSTONE, object_type::ROCK)));
	glyphs.emplace(std::make_pair('T', std::make_tuple(terrain_type::PAVEDSTONE_TALL, object_type::NONE)));
	glyphs.emplace(std::make_pair('R', std::make_tuple(terrain_type::PAVEDSTONE_TALL, object_type::ROCK)));
	glyphs.emplace(std::make_pair('w', std::make_tuple(terrain_type::WATER, object_type::NONE)));
	glyphs.emplace(std::make_pair('g', std::make_tuple(terrain_type::GRASS, object_type::NONE)));
	glyphs.emplace(std::make_pair('G', std::make_tuple(terrain_type::GRASS, object_type::ROCK)));
	glyphs.emplace(std::make_pair('s', std::make_tuple(terrain_type::SAND, object_type::CACTUS)));

	squareinfo.emplace(std::make_pair(terrain_type::LAVA, std::make_tuple("./textures/lava0.png", sf::Vector2f(0, 10), sf::IntRect())));
	squareinfo.emplace(std::make_pair(terrain_type::PAVEDSTONE, std::make_tuple("./textures/whitepaved0.png", sf::Vector2f(), sf::IntRect())));
	squareinfo.emplace(std::make_pair(terrain_type::PAVEDSTONE_TALL, std::make_tuple("./textures/whitepavedtall.png", sf::Vector2f(0, -15), sf::IntRect())));
	squareinfo.emplace(std::make_pair(terrain_type::WATER, std::make_tuple("./textures/lightwater0.png", sf::Vector2f(), sf::IntRect())));
	squareinfo.emplace(std::make_pair(terrain_type::GRASS, std::make_tuple("./textures/grass0.png", sf::Vector2f(), sf::IntRect())));
	squareinfo.emplace(std::make_pair(terrain_type::SAND, std::make_tuple("./textures/sand0.png", sf::Vector2f(), sf::IntRect())));

	entityinfo.emplace(std::make_pair(object_type::ROCK, std::make_tuple("./textures/brownrock0.png", sf::Vector2f(9, -40), sf::IntRect())));
	entityinfo.emplace(std::make_pair(object_type::CACTUS, std::make_tuple("./textures/orig.png", sf::Vector2f(22, -40), sf::IntRect(211, 1579, 44, 66))));
	entityinfo.emplace(std::make_pair(object_type::MOVESELECT, std::make_tuple("./textures/moveselect.png", sf::Vector2f(), sf::IntRect())));
	entityinfo.emplace(std::make_pair(object_type::ATTACKSELECT, std::make_tuple("./textures/attackselect.png", sf::Vector2f(), sf::IntRect())));
	entityinfo.emplace(std::make_pair(object_type::DUELIST, std::make_tuple("./textures/duelist_sheet.png", sf::Vector2f(6, -33), sf::IntRect(0, 0, 72, 72))));
	entityinfo.emplace(std::make_pair(object_type::WOLF, std::make_tuple("./textures/direwolf.png", sf::Vector2f(6, -33), sf::IntRect())));
}