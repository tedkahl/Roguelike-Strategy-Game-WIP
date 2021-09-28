#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Data.h"

/*Current structure:
* Animation

*/

struct position_getter {
private:
	sf::Time start_time;
	float speed_mult;
public:
	virtual unsigned get(sf::Time current) = 0;
	int ms(sf::Time current) const { return static_cast<int>(((current - start_time).asMilliseconds() * speed_mult)); }; //ms elapsed since start
	virtual int length() const = 0; //length of animation
	inline int count(sf::Time current) const { //how many times has animation played
		return ms(current) / length();
	};
	position_getter(sf::Time start, float speed = 1.f) :start_time(start), speed_mult(speed) {}
};

struct specify_intervals : public position_getter {
private:
	std::vector<int> intervals;
	unsigned position;
public:
	virtual unsigned get(sf::Time current) override {
		while (ms(current) % length() >= intervals[position]) position = (position + 1) % intervals.size();
		return position;
	}
	virtual int length() const override { return intervals.back(); }
	int curr_start() { return intervals[position]; }
	specify_intervals(sf::Time start, const std::vector<int>& interval, float speed = 1.f) :position_getter(start, speed), intervals(interval), position(0) {}
};

struct smooth :public position_getter {
	int anim_duration;
	int frames;
	virtual unsigned get(sf::Time current) override {

		return (ms(current) % length() * (frames / anim_duration)) % frames;
	}
	virtual int length() const override { return anim_duration; }
	smooth(sf::Time start, int duration, int num_frames, float speed = 1.f) :position_getter(start, speed), anim_duration(duration), frames(num_frames) {}
};

class Animation
{
protected:
	//LR
	sf::IntRect starting;
	std::unique_ptr<position_getter> pos;
public:
	Animation(std::unique_ptr<position_getter>&& pos_getter) :pos(std::move(pos_getter)) {}
	virtual sf::IntRect getRect(sf::Time current) = 0;
};

//animations have adjusted starting times. Intervals adjusted to animations
class Composite_Animation :public Animation {
private:
	std::vector<Animation> animations;
	int curr_start = 0;
public:
	Composite_Animation(std::unique_ptr<specify_intervals>&& pos_getter, const std::vector<Animation>& anims) :Animation(std::move(pos_getter)), animations(anims) {}

	virtual sf::IntRect getRect(sf::Time current) override {
		return animations[pos->get(current)].getRect(current);
	}
};

class LR_Animation :public Animation
{
private:
	int sheet_width;
public:
	LR_Animation(std::unique_ptr<specify_intervals>&& pos_getter, int sprite_sheet_width) : Animation(std::move(pos_getter)), sheet_width(sprite_sheet_width) {}

	virtual sf::IntRect getRect(sf::Time current) override {
		int position = pos->get(current);
		int x = (starting.left + starting.width * position);
		return sf::IntRect(x % sheet_width, (starting.top + starting.height * (x / sheet_width)), starting.width, starting.height);
	}

};
