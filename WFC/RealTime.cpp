#include "RealTime.h"
#include "Board.h"
#include "DrawComponent.h"
#include "AnimationManager.h"

using std::cout;
using std::endl;
RealTime::RealTime(Board& board) : start_time(), board_(&board) {};
RealTime::RealTime(Board& board, sf::Time speed) : start_time(), board_(&board), speed_(speed) {};
Board& RealTime::getBoard() const { return *board_; }

bool RealTime::isFirst(sf::Time current) {
	if (start_time == sf::Time()) {
		//cout << "setting start time" << endl;
		start_time = current;
		return true;
	}
	return false;
}

CompositeRT::CompositeRT(std::deque<std::unique_ptr<RealTime>>&& init, Board& board) :RealTime(board), moves(std::move(init)) {}

EntityUpdate CompositeRT::getUpdate(sf::Time current) {
	EntityUpdate ret;
	isFirst(current);
	assert(moves.size() > 0);
	ret = moves.back()->getUpdate(current);
	if (ret.finished) {
		if (moves.size() > 1) {
			auto a = std::move(ret.action);
			//start_time += moves.back()->speed();
			moves.pop_back();
			ret = moves.back()->getUpdate(current);
			assert(!ret.action);
			ret.action = std::move(a);
		}
		else moves.pop_back();
	}
	return ret;
}