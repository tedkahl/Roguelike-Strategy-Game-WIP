#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include <functional>
#include <queue>
#include "type_enums.h"
#include "Animation.h"
#include "assert.h"
#include "EntityUpdate.h"
struct Board;
class Entity;

//a sort of coroutine, which attaches to an entity and returns entityupdates based on current time
class RealTime
{
protected:
	Board* board_;
	sf::Time start_time;
	sf::Time speed_;
	inline bool isFirst(sf::Time);
public:
	RealTime() = default;
	RealTime(Board&);
	RealTime(Board&, sf::Time);
	virtual EntityUpdate getUpdate(sf::Time current) = 0;
	virtual ~RealTime() {}
	sf::Time speed() const { return speed_; }
	Board& getBoard() const;
};

//a RealTime composed of multiple other RealTimes
class CompositeRT : public RealTime {
protected:
	std::deque<std::unique_ptr<RealTime>> moves;
public:
	using RealTime::RealTime;
	CompositeRT(std::deque<std::unique_ptr<RealTime>>&& init, Board& board);
	virtual EntityUpdate getUpdate(sf::Time current) override;
	//virtual ~CompositeRT() {}
};