#include "RealTime.h"
#include "Board.h"
#include "DrawComponent.h"
#include "AnimationManager.h"

using std::cout;
using std::endl;
RealTime::RealTime(Board& board) : start_time(), board_(&board) {};
Board& RealTime::getBoard() const { return *board_; }

bool RealTime::isFirst(sf::Time current) {
	if (start_time == sf::Time()) {
		//cout << "setting start time" << endl;
		start_time = current;
		return true;
	}
	return false;
}

CompositeRT::CompositeRT(std::deque<std::unique_ptr<RealTime>>&& init, Board& board) :RealTime(board), queue(init) {}

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