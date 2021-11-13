#pragma once
#include "UnitComponent.h"
#include "Command.h"
#include "Player.h"
#include "AI.h"
#include "GameState.h"
static void handleEsc(Player& p_state) {
	if (p_state.command) p_state.deSelect();
}

static void handleInput(Level& level, sf::RenderWindow& window, GameState& g, Player& p_state, AIPlayer& enemy, sf::Event& event)
{
	auto& squares = level.state.board;
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
	{
		auto ret = level.getCoords(window, sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
		if (ret) {
			sf::Vector2i coords = ret.value();
			if (p_state.command) {
				std::cout << "executing command" << std::endl;
				auto new_state = p_state.command->execute(coords, g.now, &p_state);
				p_state.deSelect();
				p_state.deleteCommand();
			}
			else {
				Entity* unit = level.entityClickedOn(window, coords, sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
				UnitComponent* unit_uc = unit ? unit->uc() : nullptr;
				if (unit && getEnmity(unit_uc, &p_state) == enmity_t::SAME_TEAM && unit_uc->getMoveState() != unit::move_state::HAS_ACTED) {
					p_state.selected = unit;
					std::cout << "switching command" << std::endl;
					p_state.switchCommand(new AttackMove(unit, level));
					p_state.command->showTargeter();
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
				level.addEntity(rand() % 2 == 0 ? object_type::ROCK : object_type::CACTUS, 2, coords.value());

			}
			break;
		}
		case sf::Keyboard::Key::W:
		case sf::Keyboard::Key::U: {
			if (auto coords = level.getCoords(window, sf::Mouse::getPosition(window))) {
				auto& square = squares.at(coords.value());
				level.addEntityTest(event.key.code == sf::Keyboard::Key::U ? object_type::DUELIST : object_type::WOLF, event.key.code != sf::Keyboard::Key::U, coords.value());
			}
			break;
		}
		case sf::Keyboard::Key::Period: {
			if (auto unit = p_state.selected) {
				p_state.unit_wait(unit->uc());
			}
			break;
		}
		case sf::Keyboard::Key::Q: {
			if (g.active_player == -1) {
				g.active_player = 0;
				p_state.startTurn();
			}
			break;
		}/*
		case sf::Keyboard::Key::J: {
			level.displayDJ(enemy.peekMap());
		}*/
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