#include "RealTime.h"
#include "Board.h"
#include "DrawComponent.h"
#include "AnimationManager.h"

using std::cout;
using std::endl;
RealTime::RealTime(Board& board) : start_time(), board_(&board) {};
Board& RealTime::getBoard() const { return *board_; }
inline unsigned RealTime::getIndex(sf::Time current) { return static_cast<unsigned>((current - start_time) }

bool RealTime::isFirst(sf::Time current) {
	if (start_time == sf::Time()) {
		//cout << "setting start time" << endl;
		start_time = current;
		return true;
	}
	return false;
}

Lunge::Lunge(sf::Vector2i pos, sf::Vector2i target, object_type type, anim_state a_state, sf::Time start_t, Board& board, entity_action& a) : RealTime(board, animation_manager::instance()->get_basic_anim(type, a_state)), start_pos(squarePosFromCoords(pos, board_->board.width())), target_(target), dir(target - pos),
action(a) {}

EntityUpdate Lunge::getUpdate(sf::Time current) {
	std::optional<entity_action> sent_action = std::nullopt;
	std::optional<int> move_dir = std::nullopt;
	std::optional<Animation> sent_anim = std::nullopt;
	if (start_time == sf::Time()) {
		start_time = current;
		move_dir = 0;
		sent_anim = std::move(anim);
	}

	float ms = (current - start_time).asMilliseconds();
	float fraction = ms / anim.loop_length;
	if (fraction >= .5f && !hasTriggered) {
		hasTriggered = true;
		sent_action = action;
	}
	if (fraction >= 1) return EntityUpdate(true, 1, target_ - dir, start_pos);
	fraction = .5f - std::abs(.5f - fraction);
	//cout << "which becomes " << fraction << endl;
	/*up = <0, -1> = <.66, -.33>
		right = <1, 0> = <.66, .33>
		left = <-1, 0> = <-.66, -.33>
		down = <0, 1>   <-.66, .33>*/
	auto adjusted_coords = (dir.x < 0 || dir.y < 0) ? target_ - dir : target_;
	auto true_dir = sf::Vector2f(2.f / 3.f, 1.f / 3.f);
	true_dir.x *= dir.x - dir.y;
	true_dir.y *= dir.x + dir.y;

	return EntityUpdate(false, move_dir, adjusted_coords, start_pos + true_dir * fraction * lunge, std::move(sent_anim), std::move(sent_action));
}


//EntityUpdate ActionRT::getUpdate(sf::Time current) {
//	std::optional<entity_action> bound_action = std::nullopt;
//	bool first = false;
//	if (start_time == sf::Time()) {
//		start_time = current;
//		first = true;
//	}
//
//	float ms = (current - start_time).asMilliseconds();
//	float fraction = ms / anim_info[0].loop_length;
//	if (fraction >= delay && !hasTriggered) {
//		hasTriggered = true;
//		bound_action = [=, this](Entity* e) {action(target_, e); };
//	}
//	return EntityUpdate((fraction >= 1), first, std::nullopt, std::nullopt, std::nullopt, std::move(bound_action);
//}
