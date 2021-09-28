#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include "Animation.h"
#include "Board.h"
#include "DrawComponent.h"
struct Board;


struct EntityUpdate
{
	bool finished;
	std::optional<sf::Vector2i> coords_;
	std::optional<sf::Vector2f> spritePosition_;
	std::optional<sf::IntRect> textureRect_;
	EntityUpdate() :finished(true), coords_(), spritePosition_(), textureRect_() {}
};

class RealTime
{
protected:
	Board* state;
	sf::Time start_time;
	virtual sf::Vector2f getSpritePos(float pos, unsigned index) = 0;
	virtual std::optional<sf::Vector2i> getCoords(unsigned index) { return std::nullopt; }
public:
	RealTime(sf::Time start_t, Board& state_, int anim_state);
	virtual EntityUpdate getUpdate(sf::Time current) = 0;
	//virtual bool isFinished(sf::Time current) = 0;
	Board& getBoard() const;
};

class GridMove : public RealTime {
private:
	std::vector<sf::Vector2i> path_;
	sf::Time speed_;
	virtual sf::Vector2f getSpritePos(float pos, unsigned index) override;
	virtual std::optional<sf::Vector2i> getCoords(unsigned index) override;
	//sf::IntRect getTextureRect(sf::Time current);
public:
	GridMove(std::vector<sf::Vector2i>&& path, sf::Time speed, sf::Time start_t, Board& b_state, int anim_state);
	//virtual bool isFinished(sf::Time current) override;
	virtual EntityUpdate getUpdate(sf::Time current) override;
};


