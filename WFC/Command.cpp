#include "Command.h"
#include "Debug.h"
#include "Attacks.h"
#include "RealTime.h"
#include "Actions.h"
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

AttackMove::AttackMove(Entity* agent, Level& board_) : Command(agent, board_), paths(pathFind(agent_->uc(), level->state, getAttack(agent->uc()->stats().attack_type))) {
}


std::optional<unit::move_state> AttackMove::execute(sf::Vector2i target, sf::Time now, Player* p_state) {
	hideTargeter();
	sf::Vector2i move_target = target;
	bool attack = false;
	cout << "target " << to_string(target) << endl;
	cout << paths.is_attackable(target) << endl;
	auto target_enmity = getEnmity(level->state.board.at(target), agent_->uc());
	if (paths.is_attackable(target) && isNE(target_enmity)) {
		move_target = paths.grid.at(target - paths.offset).prev;

		//if move target taken, choose a new one automatically
		if (!level->state.board.at(move_target).unit()) {
			target = chooseNewTarget(getAttack(agent_->uc()->stats().attack_type), paths, target, level->state);
			if (target.x == -1)
				return std::nullopt;
		}
		cout << "Moving to attack!" << endl;
		attack = true;
	}

	auto move_path = paths.getPath(move_target);
	unit::move_state ret;
	if (move_path)
	{
		agent_->addRT(std::make_unique<GridMove>(move_path.value(), static_cast<object_type>(agent_->type()), sf::seconds(2.2f), level->state));
		ret = unit::move_state::HAS_MOVED;
		p_state->setMoveState(agent_->uc(), ret);
	}
	if (attack) {
		//maintain reference to level past object lifespan
		auto lvlRef = std::make_shared<Level*>(level);
		entity_action a = [=](Entity* e) {actions::attack(**lvlRef, target, e, e->uc()->stats().damage);};

		agent_->addRT(std::make_unique<Lunge>(move_target, target, static_cast<object_type>(agent_->type()), anim_state::ATTACKING, level->state, a));
		ret = unit::move_state::HAS_ACTED;
		p_state->setMoveState(agent_->uc(), ret);
	}
	return static_cast<unit::move_state>(ret);
}

Targeter::Targeter(pathsGrid& paths, Level* level_) : batch(-1), level(level_) {
	batch = level->addTargeter(paths);
}

Targeter::~Targeter() {
	level->removeBatch(batch);
}

pathsGrid& AttackMove::getPaths() {
	return paths;
}