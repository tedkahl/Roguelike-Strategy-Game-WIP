#include "GridMove.h"
#include "RealTime.h"
#include "Board.h"
#include "DrawComponent.h"
#include "AnimationManager.h"
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
	std::optional<Animation> sent_anim;
	//cout << "updating move" << endl;
	if (isFirst(current)) {
		sent_anim = anim;
	}
	auto coords_ = ((end_ - start_).x > 0 || (end_ - start_).y > 0) ? end_ : start_;
	float fraction = (current - start_time) / speed_;
	if (fraction >= 1.f) {//finished
		return EntityUpdate(true, 1, end_, std::nullopt);
	}
	return EntityUpdate(false, 0, coords_, getSpritePos(fraction), std::move(sent_anim));
}

//This doesn't allow for changing animation directions etc yet, but that can be done once config files are in place etc
// if path is 1,2,3,4,5 : added as 4->5, 3->4, 2->3, 1->2 : read back to front
GridMove::GridMove(const std::vector<sf::Vector2i>& path, object_type type, sf::Time speed, Board& board) :CompositeRT(board) {
	for (unsigned i = path.size() - 1;i > 0;i--) {
		moves.emplace_back(std::make_unique<SimpleMove>(path[i - 1], path[i], type, speed, board));
	}
	std::cout << "Path established: " << std::endl;
	for (auto& i : path) {
		std::cout << i.x << " " << i.y << ", ";
	}
	std::cout << "\n";
}


/*std::optional<sf::Vector2f> GridMove::getSpritePos(float pos, unsigned index) {
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
}*/