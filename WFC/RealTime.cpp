#include "RealTime.h"

RealTime::RealTime(sf::Time start_t, Board& state_, int anim_state) :start_time(start_t), state(&state_) {};

GridMove::GridMove(std::vector<sf::Vector2i>&& path, sf::Time speed, sf::Time start_t, Board& b_state, int anim_state) :RealTime(start_t, b_state, anim_state), path_(path), speed_(speed) {
	std::cout << "Path established: " << std::endl;
	for (auto i : path) {
		std::cout << i.x << " " << i.y << ", ";
	}
	std::cout << "\n";
}


//bool GridMove::isFinished(sf::Time current) {
//	/*std::cout << "RealTime finished?" << std::endl;
//	std::cout << (current - start_time).asSeconds() << std::endl;
//	std::cout << (static_cast<sf::Int64>(path_.size()) * speed_).asSeconds() << std::endl;*/
//
//	return (current - start_time) / speed_ > path_.size() - 1;
//}

EntityUpdate GridMove::getUpdate(sf::Time current) {
	float pos = (current - start_time) / speed_;
	unsigned index = static_cast<unsigned>(pos);
	if (index >= path_.size()) {//finished
		return EntityUpdate();
	}
	else return EntityUpdate{ false, getCoords(index), getSpritePos(pos, index), }

}
std::optional<sf::Vector2i> GridMove::getCoords(unsigned index) {
	//unsigned index = static_cast<unsigned>((current - start_time) / speed_);
	//std::cout << "index in path (getCoords) " << index << std::endl;
	//if moving down, use index of destination
	if (index < path_.size() - 1 && (path_[index + 1].x > path_[index].x || path_[index + 1].y > path_[index].y))
		index++;
	return path_[index];
}
//
//sf::IntRect GridMove::getTextureRect(sf::Time current) {
//	if (animation) return animation->getRect(current);
//}


sf::Vector2f GridMove::getSpritePos(float pos, unsigned index) {
	//float pos = (current - start_time) / speed_;
	//std::cout << "position in path " << pos << std::endl;
	//unsigned index = static_cast<unsigned>(pos);
	sf::Vector2f startcoords = squarePosFromCoords(path_[index], state->board.width());
	sf::Vector2f endcoords = squarePosFromCoords(path_[index + 1], state->board.width());

	float start_h = state->heightmap.at(path_[index]);
	float end_h = state->heightmap.at(path_[index + 1]);
	float fraction = (pos - index);
	startcoords = startcoords + (endcoords - startcoords) * fraction;
	startcoords.y += (fraction >= .5 ? end_h : start_h);

	return startcoords;
}

Board& RealTime::getBoard() const { return *state; }
