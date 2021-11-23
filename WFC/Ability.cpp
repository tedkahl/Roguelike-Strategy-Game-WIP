#include "Ability.h"
#include "Attacks.h"
#include "Actions.h"

Kick::Kick(Entity* agent, Level& level) : Command(agent, level), targets(pathFind(agent_->uc(), level_->state, kick_r(dist, agent->getPos(), level_->state), true)) {

}
pathsGrid& Kick::getPaths() { return targets; }

std::optional<unit::move_state> Kick::execute(sf::Vector2i target, sf::Time now, Player* p_state)
{
	auto& grid = level_->state.board;
	Entity* tgt_e = grid.at(target).unit();
	sf::Vector2i dir = target - agent_->getPos();
	if (!targets.is_attackable(target) || !isKickable(agent_->uc(), tgt_e) || sumSq(dir) != 1) return std::nullopt;
	//agent_->addRT(std::make_unique<GridMove>(move_path.value(), static_cast<object_type>(agent_->type()), sf::seconds(.2f), level_->state));
	sf::Vector2i endpos = target;
	while (targets.is_attackable(endpos + dir)) endpos += dir;

	auto lvlRef = std::make_shared<Level*>(level_);
	auto enemy = grid.at(endpos).unit_uc();
	entity_action a;
	if (enemy) {
		a = [=](Entity* e) {actions::attack(**lvlRef, enemy, e, 20);};
		endpos -= dir;
	}
	else a = actions::null_action;

	tgt_e->addRT(std::make_unique<ProjectileMove>(target, endpos, agent_->type(), anim_state::DEFAULT, sf::seconds(.5), level_->state, std::move(a)));
}
void Kick::showTargeter() {
	if (!targeter)
	{
		targeter = std::make_unique<Targeter>(targets, level_);
	}
}