#pragma once
#include "UnitComponent.h"
#include "Command.h"
#include "PlayerState.h"
//static void select(UnitComponent* unit, Level<char>& board) {
//	
//}

static void handleClick(Level<char>& level, sf::RenderWindow& window, sf::Time now, PlayerState& p_state, sf::Event& event) {
	auto ret = level.getCoords(window, sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
	if (ret) {
		sf::Vector2i coords = ret.value();
		if (p_state.command) {
			std::cout << "executing command" << std::endl;
			p_state.command->execute(coords, now);
			p_state.deSelect();
			p_state.deleteCommand();
		}
		Entity* unit = level.entityClickedOn(window, coords, sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
		if (unit) {
			p_state.selected = unit;
			std::cout << "switching command" << std::endl;
			p_state.switchCommand(new AttackMove(unit, level));
			p_state.command->showTargeter();
		}
	}
}
