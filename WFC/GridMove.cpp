#include "GridMove.h"
#include "RealTime.h"
#include "Board.h"
#include "DrawComponent.h"
#include "AnimationManager.h"
EntityUpdate CompositeRT::getUpdate(sf::Time current) {
	EntityUpdate ret;
	assert(queue.size() > 0);
	ret = queue.front()->getUpdate(current);
	if (ret.finished && queue.size() > 1) {
		ret.finished = false;
		queue.pop_front();
	}
	return ret;
}
SimpleRT::SimpleRT(object_type type, anim_state a_state, sf::Time speed, Board& board, entity_action& a) : RealTime(board), anim(animation_manager::instance()->get_basic_anim(type, a_state, speed.asMilliseconds())), speed_(speed), action(a) {}

SimpleMove::SimpleMove(sf::Vector2i start, sf::Vector2i end, object_type type, sf::Time speed, Board& board) : SimpleRT(type, anim_state::WALKING, speed, board), start_(start), end_(end) {}

std::optional<sf::Vector2f> SimpleMove::getSpritePos(float fraction) {
	sf::Vector2f startcoords = squarePosFromCoords(start_, board_->board.width());
	sf::Vector2f endcoords = squarePosFromCoords(end_, board_->board.width());

	float start_h = board_->heightmap.at(start_);
	float end_h = board_->heightmap.at(end_);
	startcoords = startcoords + (endcoords - startcoords) * fraction;
	startcoords.y += (fraction >= .5 ? end_h : start_h);

	//cout << "getting sprite pos " << startcoords.x << " " << startcoords.y << endl;
	return startcoords;
}

EntityUpdate SimpleMove::getUpdate(sf::Time current) {
	std::optional<int> dir = std::nullopt;
	std::optional<Animation> sent_anim = std::nullopt;
	//cout << "updating move" << endl;
	if (isFirst(current)) {
		dir = 0;
		sent_anim = std::move(anim);
	}
	float fraction = (current - start_time) / speed_;
	if (fraction >= 1.f) {//finished
		return EntityUpdate(true, 1, end_, std::nullopt);
	}
	return EntityUpdate(false, dir, std::nullopt, getSpritePos(fraction), std::move(sent_anim));
}




GridMove::GridMove(std::vector<sf::Vector2i>&& path, object_type type, sf::Time speed, Board& board) :RealTime(board, animation_manager::instance()->get_basic_anim(type, anim_state::WALKING)), path_(path), speed_(speed) {
	std::cout << "Path established: " << std::endl;
	for (auto& i : path) {
		std::cout << i.x << " " << i.y << ", ";
	}
	std::cout << "\n";
}


EntityUpdate GridMove::getUpdate(sf::Time current) {
	std::optional<int> dir = std::nullopt;
	std::optional<Animation> sent_anim = std::nullopt;
	//cout << "updating move" << endl;
	if (isFirst(current)) {
		dir = 0;
		sent_anim = std::move(anim);
	}
	//cout << start_time.asMilliseconds() << " " << current.asMilliseconds() << endl;
	float pos = (current - start_time) / speed_;
	//cout << "start time established pos " << pos << endl;
	unsigned index = static_cast<unsigned>(pos);
	if (index >= path_.size() - 1) {//finished
		return EntityUpdate(true, 1, path_.back(), std::nullopt);
	}
	else return EntityUpdate{ false, dir, getCoords(index), getSpritePos(pos, index) };

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

	float start_h = board_->heightmap.at(path_[index]);
	float end_h = board_->heightmap.at(path_[end_index]);
	float fraction = (pos - index);
	startcoords = startcoords + (endcoords - startcoords) * fraction;
	startcoords.y += (fraction >= .5 ? end_h : start_h);

	//cout << "getting sprite pos " << startcoords.x << " " << startcoords.y << endl;
	return startcoords;
}