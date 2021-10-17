#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include <functional>
#include "type_enums.h"
#include "Animation.h"
#include "Actions.h"
struct Board;
class Entity;
//typedef std::function<void(sf::Vector2i target_, Entity*)> entity_target_action;


/*
Immediate goal: get abilities working.
Reasonable suggestion: 1. add animation to entityupdate 2. add function to Entity that takes a single update 3. add

plan:
ActionRT: has a vector of ActionRT pointers. If set, perform them each in turn in place of own getUpdate (?)
-or just dump a bunch of one step RealTimes into Entity
-purpose: allows paths to include "hops"
-allows complex movements for abilities
-but is this a bottleneck right now

single ActionRT:
Class is defined by sprite movement. animation, action can be specified. It's exactly like this right now just rename Lunge "lunge"

EntityUpdate:
-can include animation
*/

struct EntityUpdate
{
	bool finished;
	uint8_t move_dir_;
	std::optional<sf::Vector2i> coords_;
	std::optional<sf::Vector2f> sprite_position_;
	std::optional<Animation> animation_;
	std::optional<entity_action> action;
	EntityUpdate() = default;
	EntityUpdate(bool done, uint8_t move_dir, const std::optional<sf::Vector2i>&& coords, const std::optional<sf::Vector2f>&& sprite_position,
		const std::optional<Animation>&& animation = std::nullopt, const std::optional<entity_action>&& e_action = std::nullopt) : finished(done), move_dir_(move_dir), coords_(coords), sprite_position_(sprite_position), animation_(animation), action(e_action) {}
};

//a sort of coroutine, which attaches to an entity and returns entityupdates based on current time
class RealTime
{
protected:
	Board* board_;
	sf::Time start_time;
	virtual std::optional<sf::Vector2i> getCoords(unsigned index) = 0;
	inline bool isFirst(sf::Time current);
public:
	RealTime(Board& board);
	virtual EntityUpdate getUpdate(sf::Time current) = 0;
	virtual ~RealTime() {}
	Board& getBoard() const;
};

//a RealTime composed of multiple other RealTimes
class CompositeRT : public RealTime {
protected:
	std::deque<std::unique_ptr<RealTime>> queue;
public:
	using RealTime::RealTime;
	CompositeRT(std::deque<std::unique_ptr<RealTime>>&& init, Board& board);
	virtual EntityUpdate getUpdate(sf::Time current) override;
	virtual ~CompositeRT() {}
};