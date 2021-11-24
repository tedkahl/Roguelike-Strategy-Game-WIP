#include "Ability.h"
#include "Attacks.h"
#include "Actions.h"

Kick::Kick(Entity* agent, Level& level) : Command(agent, level, pathFind(agent->uc(), level.state, kick_r(5, agent->getPos(), level.state), true)) {

}

std::optional<unit::move_state> Kick::execute(sf::Vector2i target, sf::Time now, Player* p_state)
{
	auto& grid = level_->state.board;
	Entity* tgt_e = grid.at(target).unit();
	sf::Vector2i dir = target - agent_->getPos();
	if (!paths.is_attackable(target) || !isKickable(agent_->uc(), tgt_e)) return std::nullopt;
	//agent_->addRT(std::make_unique<GridMove>(move_path.value(), static_cast<object_type>(agent_->type()), sf::seconds(.2f), level_->state));
	sf::Vector2i endpos = target;
	while (paths.is_display(endpos + dir)) endpos += dir;

	auto lvlRef = std::make_shared<Level*>(level_);
	auto enemy = grid.at(endpos).unit_uc();
	entity_action a;
	auto u = tgt_e->uc();
	if (enemy) {
		a = [=](Entity* e) {
			actions::attack(**lvlRef, enemy, agent_, 15);
			actions::attack(**lvlRef, u, agent_, 5);
		};
	}
	else {
		a = actions::null_action;
	}
	std::deque<std::unique_ptr<RealTime>> moves;

	moves.push_front(std::make_unique<ProjectileMove>(target, endpos, agent_->type(), anim_state::DEFAULT, sf::seconds(.1), level_->state, std::move(a)));
	if (enemy) //bounce off
		moves.push_front(std::make_unique<SimpleMove>(endpos, endpos - dir, agent_->type(), anim_state::DEFAULT, sf::seconds(.2), level_->state));
	tgt_e->addRT(std::make_unique<CompositeRT>(std::move(moves), level_->state));
	return unit::move_state::HAS_ACTED;
}