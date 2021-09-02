#pragma once
#include "UnitComponent.h"
#include "Paths.h"
#include "Board.h"

class Command {
protected:
	Board<char>& board;
	class targeter {
	private:
		Board<char>& board;
		BoardEntity* first;
		unsigned num;
	public:
		targeter(pathsGrid& p, Board<char>& b);
		~targeter();
	};
	UnitComponent* agent_;
	std::unique_ptr<targeter> t;
public:
	Command(UnitComponent* agent, Board<char>& board);
	virtual void showTargeter() = 0;
	virtual void execute(sf::Vector2i target, sf::Time now) = 0; //target can be changed to its own class if necessary
};

class Move :public Command {
private:
	pathsGrid paths;
public:
	Move(UnitComponent* agent, Board<char>& board_);
	virtual void showTargeter() override;
	virtual void execute(sf::Vector2i target, sf::Time now) override;
};
