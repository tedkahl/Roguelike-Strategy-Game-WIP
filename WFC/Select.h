#pragma once
#include "UnitComponent.h"
#include "Command.h"
#include "PlayerState.h"
//static void select(UnitComponent* unit, Board<char>& board) {
//	
//}

static void handleClick(Board<char>& board, sf::RenderWindow& window, sf::Time now, PlayerState& p_state, sf::Event& event) {
	auto coords = board.getCoords(window, sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
	if (coords) {
		if (p_state.command) {
			p_state.command->execute(coords.value(), now);
		}
		auto unit = board.state.board.at(coords.value()).unit();
		if (unit) {
			p_state.selected = unit;
			p_state.switchCommand(new Move(unit, board));
			p_state.command->showTargeter();
		}
	}
}
