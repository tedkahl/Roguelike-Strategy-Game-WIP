#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include <functional>
#include <iostream>
#include "assert.h"
#include "Animation.h"
class Entity;
typedef std::function<void(Entity*)> entity_action;
namespace actions {
	static void null_action(Entity* e)
	{
		cout << "null action don't call this" << endl;
		assert(false);
	}
}

struct EntityUpdate
{
	bool finished;
	uint8_t move_dir_;
	std::optional<sf::Vector2i> coords_;
	std::optional<sf::Vector2f> sprite_position_;
	std::optional<Animation> animation_;
	std::optional<entity_action> action;
	EntityUpdate() = default;
	EntityUpdate(bool done, uint8_t move_dir, const std::optional<sf::Vector2i>&& coords, const std::optional<sf::Vector2f>&& sprite_position,
		const std::optional<Animation>&& animation = std::nullopt, const std::optional<entity_action>&& e_action = std::nullopt) : finished(done), move_dir_(move_dir), coords_(coords), sprite_position_(sprite_position), animation_(animation), action(e_action) {}
};