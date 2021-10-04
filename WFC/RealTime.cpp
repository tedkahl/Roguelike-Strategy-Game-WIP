#include "RealTime.h"

using std::cout;
using std::endl;
RealTime::RealTime(sf::Time start_t, Board& board) :start_time(start_t), board_(&board) {};
RealTime::RealTime(Board& board) : start_time(), board_(&board) {};

//start time unused for now
GridMove::GridMove(std::vector<sf::Vector2i>&& path, object_type type, sf::Time speed, sf::Time start_t, Board& board) :RealTime(board), type_(type), path_(path), speed_(speed) {
	std::cout << "Path established: " << std::endl;
	for (auto& i : path) {
		std::cout << i.x << " " << i.y << ", ";
	}
	std::cout << "\n";
}

std::vector<AnimationSeg> GridMove::getAnimSegs() {
	return animation_manager::instance()->get_basic_anim(type_, anim_state::WALKING, path_.size() - 1, speed_.asMilliseconds());
}

EntityUpdate GridMove::getUpdate(sf::Time current) {
	std::optional<int> dir = std::nullopt;
	if (start_time == sf::Time()) {
		//cout << "setting start time" << endl;
		start_time = current;
		dir = 0;
	}
	float pos = (current - start_time) / speed_;
	//cout << "start time established pos " << pos << endl;
	unsigned index = static_cast<unsigned>(pos);
	if (index >= path_.size() - 1) {//finished
		return EntityUpdate(true, false, 1, path_.back(), std::nullopt);
	}
	else return EntityUpdate{ false, dir.value_or(1) == 0, dir, getCoords(index), getSpritePos(pos, index) };

}
std::optional<sf::Vector2i> GridMove::getCoords(unsigned index) {
	//unsigned index = static_cast<unsigned>((current - start_time) / speed_);
	//std::cout << "index in path (getCoords) " << index << std::endl;
	//if moving down, use index of destination
	if (index < path_.size() - 1 && (path_[index + 1].x > path_[index].x || path_[index + 1].y > path_[index].y))
		index++;
	return path_[index];
}

std::optional<sf::Vector2f> GridMove::getSpritePos(float pos, unsigned index) {
	unsigned end_index = std::min(index + 1, path_.size() - 1);
	sf::Vector2f startcoords = squarePosFromCoords(path_[index], board_->board.width());
	sf::Vector2f endcoords = squarePosFromCoords(path_[end_index], board_->board.width());
	//std::cout << pos << ", " << index << std::endl;
	float start_h = board_->heightmap.at(path_[index]);
	float end_h = board_->heightmap.at(path_[end_index]);
	float fraction = (pos - index);
	startcoords = startcoords + (endcoords - startcoords) * fraction;
	startcoords.y += (fraction >= .5 ? end_h : start_h);
	//cout << "getting sprite pos " << startcoords.x << " " << startcoords.y << endl;
	return startcoords;
}

Board& RealTime::getBoard() const { return *board_; }



MeleeAttack::MeleeAttack(sf::Vector2i pos, sf::Vector2i target, object_type type, anim_state a_state, sf::Time start_t, Board& board, entity_target_action& a) :RealTime(board), start_pos(squarePosFromCoords(pos, board_->board.width())), target_(target), dir(target - pos),
anim_info(animation_manager::instance()->get_basic_anim(type, a_state, 1)), action(a) {}


EntityUpdate MeleeAttack::getUpdate(sf::Time current) {
	std::optional<entity_action> bound_action = std::nullopt;
	std::optional<int> move_dir = std::nullopt;
	if (start_time == sf::Time()) {
		start_time = current;
		move_dir = 0;
	}

	float ms = (current - start_time).asMilliseconds();
	float fraction = ms / anim_info[0].loop_length;
	//cout << "fraction " << fraction << endl;
	if (fraction >= .5 && !hasTriggered) {
		hasTriggered = true;
		bound_action = [=, this](Entity* e) {action(target_, e); };
	}
	if (fraction >= 1) return EntityUpdate(true, false, 1, target_ - dir, start_pos);
	fraction = .5 - std::abs(.5 - fraction);
	//cout << "which becomes " << fraction << endl;
	/*up = <0, -1> = <.66, -.33>
		right = <1, 0> = <.66, .33>
		left = <-1, 0> = <-.66, -.33>
		down = <0, 1>   <-.66, .33>*/
	auto adjusted_target = (dir.x < 0 || dir.y < 0) ? target_ - dir : target_;
	auto true_dir = sf::Vector2f(2.f / 3.f, 1.f / 3.f);
	true_dir.x *= dir.x - dir.y;
	true_dir.y *= dir.x + dir.y;
	//cout << to_string(dir) << endl;
	//cout << to_string(true_dir) << endl;
	return EntityUpdate(false, move_dir.value_or(1) == 0, move_dir, adjusted_target, start_pos + true_dir * fraction * lunge, std::move(bound_action));
}

std::vector<AnimationSeg> MeleeAttack::getAnimSegs() {
	return anim_info;
}