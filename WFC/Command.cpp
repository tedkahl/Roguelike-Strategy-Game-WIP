#include "Command.h"
#include "Debug.h"
#include "Attacks.h"
Command::Command(Entity* agent, Level& level_) :agent_(agent), level(&level_) {}

void AttackMove::showTargeter() {
	if (!targeter)
	{
		targeter = std::make_unique<Targeter>(paths, level);
	}
}
void AttackMove::hideTargeter() {
	delete targeter.release(); //release ownership of Targeter and delete underlying pointer
}

AttackMove::AttackMove(Entity* agent, Level& board_) : Command(agent, board_), paths(pathFind(agent_->uc(), level->state, getAttack<map_node>(agent->uc()->stats().attack_type))) {
	//for (int i = 0;i < paths.grid.width();i++) {
	//	for (int j = 0;j < paths.grid.height();j++) {
	//		if (paths.grid.at(i, j).search == paths.search) {
	//			std::cout << "Reachable: " << to_string(paths.offset + sf::Vector2i(i, j)) << std::endl;
	//		}
	//	}
	//}
}

std::optional<unit::move_state> AttackMove::execute(sf::Vector2i target, sf::Time now, PlayerState* p_state) {
	hideTargeter();
	sf::Vector2i move_target = target;
	bool attack = false;
	cout << "target " << to_string(target) << endl;
	cout << paths.is_attackable(target) << endl;
	auto target_enmity = getEnmity(level->state.board.at(target).unit_uc(), agent_->uc());
	if (paths.is_attackable(target) && isNE(target_enmity)) {
		move_target = paths.grid.at(target - paths.offset).prev;
		if (paths.grid.at(move_target).has_ally) {
			//look for new target
			return std::nullopt;
		}
		cout << "Moving to attack!" << endl;
		attack = true;
	}

	auto move_path = paths.getPath(move_target);
	unit::move_state ret;
	if (move_path)
	{
		agent_->addRT(std::make_unique<GridMove>(std::move(move_path.value()), static_cast<object_type>(agent_->type_), sf::seconds(.2), now, level->state));
		ret = unit::move_state::HAS_MOVED;
		p_state->setMoveState(agent_->uc(), ret);
	}
	if (attack) {
		//maintain reference to level past object lifespan
		auto lvlRef = std::make_shared<Level*>(level);
		entity_target_action a = [=](sf::Vector2i target_, Entity* e) {actions::attack(**lvlRef, target_, e);};

		agent_->addRT(std::make_unique<MeleeAttack>(move_target, target, static_cast<object_type>(agent_->type_), anim_state::ATTACKING, now, level->state, a));
		ret = unit::move_state::HAS_ACTED;
		p_state->setMoveState(agent_->uc(), ret);
	}
	return static_cast<unit::move_state>(ret);
}

Targeter::Targeter(pathsGrid& paths, Level* level_) : batch(-1), level(level_) {
	batch = level->addTargeter(paths);
}

Targeter::~Targeter() {
	level->removeTargeter(batch);
}

pathsGrid& AttackMove::getPaths() {
	return paths;
}