#include "Movement.h"

static sf::Vector2f squarePosFromCoords(sf::Vector2i pos, BoardState& state) {
	return sf::Vector2f((state.board.width() - 1 + pos.x - pos.y) * sq::square_w / 2, (pos.x + pos.y) * sq::square_h / 2);
}


GridMove::GridMove(std::vector<sf::Vector2i>&& path, sf::Time speed, sf::Time start_t, BoardState& b_state) :Movement(start_t), state(b_state), path_(path), speed_(speed) {

}
bool GridMove::isFinished(sf::Time current) {
	return (current - start_time) > static_cast<sf::Int64>(path_.size()) * speed_;
}
sf::Vector2i GridMove::getCoordChange(sf::Time current) {
	float pos = (current - start_time) / speed_;
	return path_[static_cast<unsigned>(pos)] - path_[0];
}

sf::Vector2f GridMove::getSpritePos(sf::Time current) {
	float pos = (current - start_time) / speed_;
	unsigned index = static_cast<unsigned>(pos);
	sf::Vector2f startcoords = squarePosFromCoords(path_[index], state);
	sf::Vector2f endcoords = squarePosFromCoords(path_[index], state);
	float start_h = state.board.at(path_[index]).dc()->getOffset().y;
	float end_h = state.board.at(path_[index + 1]).dc()->getOffset().y;
	float fraction = (pos - index);
	startcoords = startcoords + (endcoords - startcoords) * fraction;
	startcoords.y += (fraction >= .5 ? end_h : start_h);
	return startcoords;
}

