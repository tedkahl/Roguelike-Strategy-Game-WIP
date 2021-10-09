#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include <functional>
#include "type_enums.h"
#include "Animation.h"
struct Board;
class Entity;
//typedef std::function<void(Entity*, sf::Vector2i, Board&)> entity_action;
typedef std::function<void(Entity*)> entity_action;
typedef std::function<void(sf::Vector2i target_, Entity*)> entity_target_action;

struct EntityUpdate
{
	bool finished;
	bool first;
	std::optional<int> move_dir;
	std::optional<sf::Vector2i> coords_;
	std::optional<sf::Vector2f> sprite_position_;
	std::optional<entity_action> action;
	EntityUpdate(bool done, bool is_first, std::optional<int> dir, const std::optional<sf::Vector2i>&& coords, const std::optional<sf::Vector2f>&& sprite_position, const std::optional<entity_action>&& e_action = std::nullopt) : finished(done), first(is_first), move_dir(dir), coords_(coords), sprite_position_(sprite_position), action(e_action) {}
};

class RealTime
{
protected:
	Board* board_;
	sf::Time start_time;
	virtual std::optional<sf::Vector2i> getCoords(unsigned index) { return std::nullopt; }
public:
	RealTime(sf::Time start_t, Board& board_);
	RealTime(Board& board_);
	virtual EntityUpdate getUpdate(sf::Time current) = 0;
	virtual std::vector<AnimationSeg> getAnimSegs() = 0;
	Board& getBoard() const;
};

class GridMove : public RealTime {
private:
	std::vector<sf::Vector2i> path_;
	sf::Time speed_;
	std::optional<sf::Vector2f> getSpritePos(float pos, unsigned index);
	virtual std::optional<sf::Vector2i> getCoords(unsigned index) override;
	object_type type_;
	//sf::IntRect getTextureRect(sf::Time current);
public:
	GridMove(std::vector<sf::Vector2i>&& path, object_type type, sf::Time speed, sf::Time start_t, Board& board);
	virtual std::vector<AnimationSeg> getAnimSegs() override;
	virtual EntityUpdate getUpdate(sf::Time current) override;
};

class MeleeAttack : public RealTime {
private:
	const float lunge = 15.f;
	sf::Vector2f start_pos;
	sf::Vector2i dir;
	sf::Vector2i target_;
	std::vector<AnimationSeg> anim_info;
	entity_target_action action;
	bool hasTriggered = false;

public:
	MeleeAttack(sf::Vector2i pos, sf::Vector2i target, object_type type, anim_state state, sf::Time start_t, Board& board, entity_target_action& a);
	//virtual bool isFinished(sf::Time current) override;
	virtual EntityUpdate getUpdate(sf::Time current) override;
	virtual std::vector<AnimationSeg> getAnimSegs() override;
};


