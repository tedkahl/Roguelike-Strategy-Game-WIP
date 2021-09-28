#include "Command.h"


Command::Command(Entity* agent, Level<char>& level_) :agent_(agent), level(level_) {}

void AttackMove::showTargeter() {
	if (!targeter)
	{
		targeter = std::make_unique<Targeter>(paths, level);
	}
}
void AttackMove::hideTargeter() {
	delete targeter.release(); //release ownership of Targeter and delete underlying pointer
}

AttackMove::AttackMove(Entity* agent, Level<char>& board_) : Command(agent, board_), paths(pathFind(agent_->uc(), level.state)) {
	//for (int i = 0;i < paths.grid.width();i++) {
	//	for (int j = 0;j < paths.grid.height();j++) {
	//		if (paths.grid.at(i, j).search == paths.search) {
	//			std::cout << "Reachable: " << to_string(paths.offset + sf::Vector2i(i, j)) << std::endl;
	//		}
	//	}
	//}
}

void AttackMove::execute(sf::Vector2i target, sf::Time now) {
	hideTargeter();
	sf::Vector2i move_target = target;
	if (paths.is_attackable(target) && level.state.board.at(target).unit() && isEnemy(level.state.board.at(target).unit()->uc(), agent_->uc())) {
		move_target = paths.grid.at(target).prev;
	}
	auto move_path = paths.getPath(move_target);

	if (move_path)
	{
		agent_->addMovement(std::make_unique<GridMove>(std::move(move_path.value()), sf::seconds(.2), now, level.state));
	}
}

Targeter::Targeter(pathsGrid& paths, Level<char>& level_) : batch(-1), level(level_) {
	batch = level.addTargeter(paths);
}

Targeter::~Targeter() {
	level.removeTargeter(batch);
}