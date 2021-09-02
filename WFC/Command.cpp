#include "Command.h"
Command::Command(UnitComponent* agent, Board<char>& board_) :agent_(agent), board(board_) {}

Move::Move(UnitComponent* agent, Board<char>& board_) : Command(agent, board_) {
}

void Move::showTargeter() {
	paths = pathFind(agent_, board.state);
	t = std::make_unique<targeter>(paths, board);
}

void Move::execute(sf::Vector2i target, sf::Time now) {
	delete t.release(); //release ownership of targeter and delete underlying poiter
	auto path = paths.getPath(target);
	if (path)
		agent_->getOwner()->addMovement(new GridMove(std::move(path.value()), sf::seconds(.2), now, board.state));
}

Command::targeter::targeter(pathsGrid& paths, Board<char>& b) : board(b) {
	for (int i = paths.minX;i <= paths.maxX;i++) {
		for (int j = paths.minY;j <= paths.maxY;j++) {
			if (paths.grid->at(i, j).search == paths.search) {
				BoardEntity* ptr = b.addEntity(object_type::MOVESELECT, sf::Vector2i(i, j));
				num++;
				if (!first) first = ptr;
			}
		}
	}
}

Command::targeter::~targeter() {
	for (int i = 0;i < num;i++) {
		assert(board.removeEntity(first + i));
	}
}