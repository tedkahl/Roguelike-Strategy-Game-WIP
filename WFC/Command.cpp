#include "Command.h"
#include "Debug.h"
#include "Attacks.h"
#include "PathingGrids.h"
#include "RealTime.h"
#include "Actions.h"
Command::Command(Entity* agent, Level& level) :agent_(agent), level_(&level) {}


void AttackMove::showTargeter() {
	if (!targeter)
	{
		targeter = std::make_unique<Targeter>(paths, level_);
	}
}
void Command::hideTargeter() {
	delete targeter.release(); //release ownership of Targeter and delete underlying pointer
}

AttackMove::AttackMove(Entity* agent, Level& level) : Command(agent, level), paths(pathFind(agent_->uc(), level_->state, getAttack(agent->uc()->stats().attack_type))) {
}
AttackMove::AttackMove(Entity* agent, Level& level, pathsGrid&& known_paths) : Command(agent, level), paths(std::move(known_paths)) {}


std::optional<unit::move_state> AttackMove::execute(sf::Vector2i target, sf::Time now, Player* p_state) {
	hideTargeter();
	auto& board = level_->state.board;
	sf::Vector2i move_target = target;
	bool attack = false;
	cout << "my loc " << to_string(agent_->getPos()) << endl;
	cout << "target " << to_string(target) << endl;
	auto target_enmity = getEnmity(board.at(target), agent_->uc());
	if (paths.is_attackable(target) && isNE(target_enmity)) {
		move_target = paths.grid.at(target - paths.offset).prev;

		//if move target taken, choose a new one automatically
		if (board.at(move_target).unit() && board.at(move_target).unit() != agent_) {
			target = chooseNewTarget(getAttack(agent_->uc()->stats().attack_type), paths, target, level_->state);
			if (target.x == -1)
				return std::nullopt;
		}
		cout << "Moving to attack!" << endl;
		attack = true;
	}

	auto move_path = paths.getPath(move_target);
	if (!move_path) return std::nullopt;
	unit::move_state ret;
	if (move_path)
	{
		agent_->addRT(std::make_unique<GridMove>(move_path.value(), static_cast<object_type>(agent_->type()), sf::seconds(.2f), level_->state));
		ret = unit::move_state::HAS_MOVED;
		p_state->setMoveState(agent_->uc(), ret);
	}
	if (attack) {
		//maintain reference to level past object lifespan
		auto lvlRef = std::make_shared<Level*>(level_);
		auto enemy = board.at(target).unit_uc();
		entity_action a = [=](Entity* e) {actions::attack(**lvlRef, enemy, e, e->uc()->stats().damage);};

		agent_->addRT(std::make_unique<Lunge>(move_target, target, static_cast<object_type>(agent_->type()), anim_state::ATTACKING, level_->state, std::move(a)));
		ret = unit::move_state::HAS_ACTED;
		p_state->setMoveState(agent_->uc(), ret);
	}
	return static_cast<unit::move_state>(ret);
}

template<typename T>
Targeter::Targeter(const T& targets, Level* level) :batch_(-1), level_(level)
{
	batch_ = level_->getDCManager().addBatch();
	DrawComponent* obj_dc;
	for (unsigned y = 0;y < targets.grid.height();y++) {
		for (unsigned x = 0;x < targets.grid.width();x++) {
			if (targets.grid.at(x, y).search == targets.search) {
				auto type = getTarget(targets, { static_cast<int>(x), static_cast<int>(y) });
				/*if (type == target_t::ATTACK_TGT) {
					assert(paths.grid.at(x, y).attackable);
				}*/
				obj_dc = getObjDC(level_->getDCManager(), level_->getTM(), type, batch_);
				obj_dc->updateEntityPos(targets.offset + sf::Vector2i{ static_cast<int>(x), static_cast<int>(y) }, level_->state.heightmap);
			}
		}
	}
}

Targeter::~Targeter() {
	level_->removeBatch(batch_);
}

pathsGrid& AttackMove::getPaths() {
	return paths;
}