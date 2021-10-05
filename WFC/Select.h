#pragma once
#include "UnitComponent.h"
#include "Command.h"
#include "PlayerState.h"
static void handleEsc(PlayerState& p_state) {
	if (p_state.command) p_state.deSelect();
}

static void unit_wait(UnitComponent* u) {
	u->movestate = UnitComponent::move_state::HAS_ACTED;
}

static void handleInput(Level& level, sf::RenderWindow& window, sf::Time now, PlayerState& p_state, sf::Event& event)
{
	auto& squares = level.state.board;
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
	{
		auto ret = level.getCoords(window, sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
		if (ret) {
			sf::Vector2i coords = ret.value();
			if (p_state.command) {
				std::cout << "executing command" << std::endl;
				p_state.command->execute(coords, now);
				p_state.deSelect();
				p_state.deleteCommand();
			}
			else {
				Entity* unit = level.entityClickedOn(window, coords, sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
				UnitComponent* unit_uc = unit ? unit->uc() : nullptr;
				if (unit && !isEnemy(unit_uc, &p_state) && unit_uc->movestate != UnitComponent::move_state::HAS_ACTED) {
					p_state.selected = unit;
					std::cout << "switching command" << std::endl;
					if (unit_uc->movestate == UnitComponent::move_state::FREE)
					{
						p_state.switchCommand(new AttackMove(unit, level));
						p_state.command->showTargeter();
					}
				}
			}
		}
	}
	else if (event.type == sf::Event::KeyPressed) {
		std::cout << sf::Mouse::getPosition().x << " " << sf::Mouse::getPosition().y << std::endl;
		switch (event.key.code) {
		case sf::Keyboard::Key::A: {
			if (auto coords = level.getCoords(window, sf::Mouse::getPosition(window))) {
				auto& square = squares.at(coords.value());
				level.addEntity(rand() % 2 == 0 ? object_type::ROCK : object_type::CACTUS, coords.value());

			}
			break;
		}
		case sf::Keyboard::Key::W:
		case sf::Keyboard::Key::U: {
			if (auto coords = level.getCoords(window, sf::Mouse::getPosition(window))) {
				auto& square = squares.at(coords.value());
				level.addEntity(event.key.code == sf::Keyboard::Key::U ? object_type::DUELIST : object_type::WOLF, coords.value());
			}
			break;
		}
		case sf::Keyboard::Key::Period: {
			if (auto unit = p_state.selected) {
				unit_wait(unit->uc());
			}
			break;
		}
		case sf::Keyboard::Key::D: {
			if (auto coords = level.getCoords(window, sf::Mouse::getPosition(window))) {
				auto& square = squares.at(coords.value());
				if (square.entities.size() != 0) {
					auto e = square.entities[0];
					level.removeEntity(e);
				}
				else if (square.unit()) {
					auto e = square.unit();
					level.removeEntity(e);
				}
				else {
					std::cout << "no entity!" << std::endl;
				}
			}
			break;
		}
		case sf::Keyboard::Key::Escape: {
			handleEsc(p_state);
		}
		}
	}
}