#include "RTAction.h"
#include "AnimationManager.h"
/*up = <0, -1> = <.66, -.33>
	right = <1, 0> = <.66, .33>
	left = <-1, 0> = <-.66, -.33>
	down = <0, 1>   <-.66, .33>*/
static sf::Vector2f getTrueDir(sf::Vector2i dir) {
	return { (2.f / 3.f) * (dir.x - dir.y), (1.f / 3.f) * (dir.x | dir.y) };
}
Lunge::Lunge(sf::Vector2i pos, sf::Vector2i target, object_type type, anim_state a_state, sf::Time start_t, Board& board, entity_action& a) : SimpleRT(type, a_state, speed, board, a), start_pos(squarePosFromCoords(pos, board_->board.width())), target_(target), dir(target - pos),
action(a) {}

EntityUpdate SimpleRT::getUpdate(sf::Time current) {
	std::optional<entity_action> sent_action = std::nullopt;
	std::optional<Animation> sent_anim = std::nullopt;
	if (isFirst(current)) {
		sent_anim = anim;
	}

	float fraction = (current - start_time).asMilliseconds() / anim.loop_length;
	if (fraction >= delay_ && !hasTriggered) {
		hasTriggered = true;
		sent_action = action;
	}
	if (fraction >= 1) return EntityUpdate(true, 1, std::nullopt, std::nullopt, std::nullopt, std::move(sent_action));
	return EntityUpdate(false, 0, std::nullopt, std::nullopt, std::move(sent_anim), std::move(sent_action));
}

EntityUpdate Lunge::getUpdate(sf::Time current) {
	auto update = SimpleRT::getUpdate(current);
	float fraction = (current - start_time).asMilliseconds() / anim.loop_length;
	fraction = .5f - std::abs(.5f - fraction);
	auto true_dir = getTrueDir(dir);

	update.coords_ = (dir.x < 0 || dir.y < 0) ? target_ - dir : target_;
	update.sprite_position_ = start_pos + true_dir * fraction * lunge;

	return EntityUpdate(update);
}
SimpleRT::SimpleRT(object_type type, anim_state a_state, sf::Time speed, Board& board, entity_action& a, float delay) : RealTime(board),
anim(animation_manager::instance()->get_basic_anim(type, a_state, speed.asMilliseconds())), speed_(speed), action(a), delay_(delay) {}


