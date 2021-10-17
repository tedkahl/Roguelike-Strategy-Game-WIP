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
	std::optional<int> move_dir;
	std::optional<sf::Vector2i> coords_;
	std::optional<sf::Vector2f> sprite_position_;
	std::optional<Animation> animation_;
	std::optional<entity_action> action;
	EntityUpdate() = default;
	EntityUpdate(bool done, std::optional<int> dir, const std::optional<sf::Vector2i>&& coords, const std::optional<sf::Vector2f>&& sprite_position,
		const std::optional<Animation>&& animation = std::nullopt, const std::optional<entity_action>&& e_action = std::nullopt) : finished(done), move_dir(dir), coords_(coords), sprite_position_(sprite_position), animation_(animation), action(e_action) {}
};

class RealTime
{
protected:
	Board* board_;
	sf::Time start_time;
	virtual std::optional<sf::Vector2i> getCoords(unsigned index) { return std::nullopt; }
	inline bool isFirst(sf::Time current);
	inline unsigned getIndex(sf::Time current);
public:
	RealTime(Board& board_);
	virtual EntityUpdate getUpdate(sf::Time current) = 0;
	virtual ~RealTime() {}
	Board& getBoard() const;
};

class Lunge : public RealTime {
private:
	const float lunge = 15.f;
	sf::Vector2f start_pos;
	sf::Vector2i dir;
	sf::Vector2i target_;
	entity_action action;
	bool hasTriggered = false;

public:
	Lunge(sf::Vector2i pos, sf::Vector2i target, object_type type, anim_state state, sf::Time start_t, Board& board, entity_action& a);
	virtual EntityUpdate getUpdate(sf::Time current) override;
};
