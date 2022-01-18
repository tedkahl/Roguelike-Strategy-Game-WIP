#include "Ability.h"
#include "Attacks.h"
#include "Actions.h"
constexpr int dist = 5;
Kick::Kick(Entity* agent, Level& level) : Command(agent, level, pathFind(agent->uc(), level.state, kick_r(5), true)) {}
PoisonArrow::PoisonArrow(Entity* agent, Level& level) : Command(agent, level, pathFind(agent->uc(), level.state, arrow_r(3, 5), true)) {}
Dash::Dash(Entity* agent, Level& level) : Command(agent, level, pathFind(agent->uc(), level.state, arrow_r(3, 5), true)) {}

//there is more to do here. Kicking into walls, pits, etc. For now 0-distance kicks are invalid
std::optional<unit::move_state> Kick::execute(sf::Vector2i target, sf::Time now, Player* p_state)
{
	auto& grid = level_->state.board;
	Entity* tgt_e = grid.at(target).unit();
	sf::Vector2i dir = target - agent_->getPos();
	if (!paths.is_attackable(target) || !isKickable(agent_->uc(), tgt_e)) return std::nullopt;
	//agent_->addRT(std::make_unique<GridMove>(move_path.value(), static_cast<object_type>(agent_->type()), sf::seconds(.2f), level_->state));
	sf::Vector2i endpos = target;
	while (paths.is_display(endpos + dir)) endpos += dir;
	if (endpos == target) return std::nullopt;

	auto enemy = (endpos != target) ? grid.at(endpos).unit_uc() : nullptr;
	entity_action a;
	if (enemy)
	{
		auto lvlRef = std::make_shared<Level*>(level_);
		auto u = tgt_e->uc();
		auto agent = agent_;
		a = [&, lvlRef, enemy, u, agent](Entity* e) {
			actions::attack(**lvlRef, enemy, agent, 15);
			actions::attack(**lvlRef, u, agent, 5);
		};
	}
	else  a = actions::null_action;

	std::deque<std::unique_ptr<RealTime>> moves;

	moves.push_front(std::make_unique<ProjectileMove>(target, endpos, agent_->type(), anim_state::DEFAULT, sf::seconds(.1), level_->state, std::move(a)));
	if (enemy) //bounce off
		moves.push_front(std::make_unique<SimpleMove>(endpos, endpos - dir, agent_->type(), anim_state::DEFAULT, sf::seconds(.2), level_->state));
	tgt_e->addRT(std::make_unique<CompositeRT>(std::move(moves), level_->state));
	return unit::move_state::HAS_ACTED;
}
//fix targeting clicks
std::optional<unit::move_state> PoisonArrow::execute(sf::Vector2i target, sf::Time now, Player* p_state)
{
	auto& grid = level_->state.board;
	Entity* tgt_e = grid.at(target).unit();

	if (!paths.is_attackable(target) || !tgt_e || !isNE(getEnmity(agent_->uc(), tgt_e->uc()))) return std::nullopt;
	auto lvlRef = std::make_shared<Level*>(level_);
	auto enemy = tgt_e->uc();
	auto agent = agent_;

	entity_action a = [&, lvlRef, enemy, agent](Entity* e) {
		actions::attack(**lvlRef, enemy, agent, 12);
	};
	Entity* arrow = level_->addEntity(object_type::ARROW, 3, agent_->getPos());
	auto shoot = std::make_unique<ProjectileMove>(arrow->getPos(), target, arrow->type(), anim_state::WALKING, sf::seconds(.2f), level_->state, std::move(a), true);
	shoot->set_arcing(1, 5, (level_->state.heightmap.at(target) - level_->state.heightmap.at(agent_->getPos())));
	arrow->addRT(std::move(shoot));

	return unit::move_state::HAS_ACTED;
}

sf::Vector2i get_dir(sf::Vector2i start, sf::Vector2i end) {
	auto delta = end - start;
	return sf::Vector2i(std::clamp(delta.x, -1, 1), std::clamp(delta.y, -1, 1));
}

std::optional<unit::move_state> Dash::execute(sf::Vector2i target, sf::Time now, Player* p_state)
{
	auto& grid = level_->state.board;
	Entity* tgt_e = grid.at(target).unit();

	if (!paths.is_attackable(target) || !tgt_e || !isNE(getEnmity(agent_->uc(), tgt_e->uc()))) return std::nullopt;
	auto lvlRef = std::make_shared<Level*>(level_);
	auto agent = agent_;
	auto start_pos = agent->getPos();

	entity_action a = [&, lvlRef, start_pos, target, agent](Entity* e) {
		auto dir = get_dir(start_pos, target);
		std::vector<sf::Vector2i> targets;
		for (sf::Vector2i p = start_pos + dir; p != target + dir; p += dir) targets.push_back(p);
		actions::attackArea(**lvlRef, targets, agent, 6);
	};

	agent_->addRT(std::make_unique<ProjectileMove>(agent_->getPos(), target, agent_->type(), anim_state::DEFAULT, sf::seconds(.2f), level_->state, std::move(a), false));

	return unit::move_state::HAS_ACTED;
}