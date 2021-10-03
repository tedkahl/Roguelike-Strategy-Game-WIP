#include "Command.h"


Command::Command(Entity* agent, Level& level_) :agent_(agent), level(level_) {}

void AttackMove::showTargeter() {
	if (!targeter)
	{
		targeter = std::make_unique<Targeter>(paths, level);
	}
}
void AttackMove::hideTargeter() {
	delete targeter.release(); //release ownership of Targeter and delete underlying pointer
}

AttackMove::AttackMove(Entity* agent, Level& board_) : Command(agent, board_), paths(pathFind(agent_->uc(), level.state)) {
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
	bool attack = false;
	cout << "target " << to_string(target) << endl;
	cout << paths.is_attackable(target) << endl;
	cout << isEnemy(level.state.board.at(target).unit_uc(), agent_->uc()) << endl;

	if (paths.is_attackable(target) && isEnemy(level.state.board.at(target).unit_uc(), agent_->uc())) {
		cout << "Moving to attack!" << endl;
		move_target = paths.grid.at(target - paths.offset).prev;
		attack = true;
	}
	auto move_path = paths.getPath(move_target);

	if (move_path)
	{
		agent_->addRT(std::make_unique<GridMove>(std::move(move_path.value()), static_cast<object_type>(agent_->type_), sf::seconds(.2), now, level.state));
		agent_->uc()->movestate = UnitComponent::move_state::HAS_MOVED;
	}
	if (attack) {
		agent_->addRT(std::make_unique<MeleeAttack>(move_target, target, static_cast<object_type>(agent_->type_), anim_state::ATTACKING, now, level.state));
		agent_->uc()->movestate = UnitComponent::move_state::HAS_ACTED;
	}
}

Targeter::Targeter(pathsGrid& paths, Level& level_) : batch(-1), level(level_) {
	batch = level.addTargeter(paths);
}

Targeter::~Targeter() {
	level.removeTargeter(batch);
}