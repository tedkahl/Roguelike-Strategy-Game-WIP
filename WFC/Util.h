#pragma once
#include <SFML/Graphics.hpp>
#include "Data.h"
#include "Random.h"
#include "DataManager.h"
#include "SortedDManager.h"
#include "UnitComponent.h"

static UnitComponent* makeUnit(DataManager<UnitComponent>& units, int team, object_type t) {
	assert(isUnitOrObj(t));
	switch (t) {
	case object_type::WOLF: {
		return units.declareNew(UnitStats(move_type::WALK, attack_type::MELEE, 1, 6, 10 + roll(1, 3), 4), team);
	}
	case object_type::DUELIST: {
		return units.declareNew(UnitStats(move_type::WALK, attack_type::MELEE, 1, 4, 18 + roll(2, 2), 6), team);
	}
	case object_type::ROCK: {
		return units.declareNew(UnitStats(move_type::WALK, attack_type::NO_ATTACK, 0, 0, 50, 0), team);
	}
	case object_type::CACTUS: {
		return units.declareNew(UnitStats(move_type::WALK, attack_type::NO_ATTACK, 0, 0, 30, 0), team);
	}
	default: assert(false);
	}
	return nullptr;
}

static DrawComponent* getObjDC(SortedDManager<DrawComponent>& dcomponents, ResourceManager<sf::Texture>& tm, object_type t) {
	auto search = Data<char>::d()->entityinfo.find(t);
	if (search != Data<char>::d()->entityinfo.end()) {
		auto& [path, offset, rect] = search->second;
		return dcomponents.declareNew(tm.get(path), offset, 1 + isTall(t) * 49, rect);
	}
	return nullptr;
}

static DrawComponent* getObjDCBatched(SortedDManager<DrawComponent>& dcomponents, ResourceManager<sf::Texture>& tm, object_type t, unsigned batch) {
	auto search = Data<char>::d()->entityinfo.find(t);
	if (search != Data<char>::d()->entityinfo.end()) {
		auto& [path, offset, rect] = search->second;
		return dcomponents.declareNewBatched(tm.get(path), offset, 1 + isTall(t) * 49, rect, batch);
	}
	return nullptr;
}

//static sf::RenderTexture&& get_DJ_Square(sf::Font& font, float score, ResourceManager<sf::Texture>& tm, object_type t) {
//	sf::RenderTexture texture;
//	auto search = Data<char>::d()->entityinfo.find(t);
//	if (search != Data<char>::d()->entityinfo.end()) {
//		auto& [path, offset, rect] = search->second;
//		sf::Sprite s(tm.get(path));
//		s.setColor({ 0, static_cast<uint8_t>(255 * score),static_cast<uint8_t>(255 * (1 - score)) });
//		texture.create(static_cast<unsigned>(s.getGlobalBounds().width), static_cast<unsigned>(s.getGlobalBounds().height));
//		texture.draw(s);
//
//		sf::Text ftext;
//		ftext = sf::Text(std::to_string(static_cast<int>(score * 100)), font, 16);
//		ftext.setPosition(static_cast<sf::Vector2f>(texture.getSize()) / 3.f);
//		ftext.setFillColor(sf::Color::Black);
//		texture.draw(ftext);
//		texture.display();
//	}
//	//return dcomponents.declareNewBatched(, offset, 1 + isTall(t), rect, batch);
//	return std::move(texture);
//}



