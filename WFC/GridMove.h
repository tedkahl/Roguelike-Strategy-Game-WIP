#pragma once
#include "RealTime.h"
#include "RTAction.h"

class GridMove : public CompositeRT {
private:
	//std::optional<sf::Vector2f> getSpritePos(float pos, unsigned index);
public:
	GridMove() = default;
	GridMove(const std::vector<sf::Vector2i>& path, object_type type, sf::Time speed, Board& board);
	using CompositeRT::getUpdate;
};


class SimpleMove :public SimpleRT {
private:
	sf::Vector2i start_;
	sf::Vector2i end_;
	std::optional<sf::Vector2f> getSpritePos(float fraction);
public:
	SimpleMove() = default;
	SimpleMove(sf::Vector2i start, sf::Vector2i end, object_type, anim_state, sf::Time speed, Board& board);
	virtual EntityUpdate getUpdate(sf::Time current) override;
};

class ProjectileMove :public SimpleRT {
private:
	bool del_when_done_;
	sf::Vector2i start_;
	sf::Vector2i end_;
	double dist;
	double h_speed; //in squares/second
	double v_speed; //in squares/second
	double g_ = 0;
	sf::Vector2f getSpritePos(float fraction, double vertical);
public:
	ProjectileMove() = default;
	ProjectileMove(sf::Vector2i start, sf::Vector2i end, object_type, anim_state, sf::Time speed, Board& board, entity_action&& a = actions::null_action, bool del_when_done = false);
	void set_arcing(double theta, double g, double delta_h);
	virtual EntityUpdate getUpdate(sf::Time current) override;
};
