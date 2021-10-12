#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <array>
#include <iostream>
#include <algorithm>
#include "Debug.h"
using std::cout;
using std::endl;

//start at a given rect, and move right (mod width) for a certain number of frames
struct AnimationSeg {
	sf::IntRect starting;
	unsigned short loop_length; //ms for now
	uint8_t frames;
	uint8_t loops;
	inline float frame_duration() const { return static_cast<float>(loop_length) / frames; }
	inline unsigned short length() const { return loop_length * loops; }
	void print() {
		cout << to_str(starting);
		cout << "ll: " << loop_length << " frames: " << static_cast<int>(frames) << " loops: " << static_cast<int>(loops) << endl << endl;
	}
	AnimationSeg() = default;
	AnimationSeg(const sf::IntRect& start, unsigned short loop_len, uint8_t num_frames, uint8_t num_loops) :starting(start), loop_length(loop_len), frames(num_frames), loops(num_loops) {}
};

//store a series of animation segments and get texture rectangle from them. 
template<size_t SEG_MAX>
struct Animation {
private:
	sf::Time start_time;

	float ms(sf::Time current) const { return static_cast<int>(((current - start_time).asMilliseconds() - time_mod) * speed_mult); }
	uint8_t segments;
	float speed_mult;
	bool loop_;
	uint8_t index;
	unsigned short time_mod; //time taken by previous segments
	std::array<AnimationSeg, SEG_MAX> animation;
public:
	void set(sf::Time start, std::vector<AnimationSeg>&& segs, bool loop = false, float speed = 1.f) {
		start_time = start;
		segments = segs.size();
		for (unsigned i = 0;i < segs.size();i++) {
			animation[i] = segs[i];
		}
		loop_ = loop;
		speed_mult = speed;
		time_mod = 0;
		index = 0;
		/*cout << "initiating animation" << endl;
		int i = 0;
		for (int i = 0;i < segments;i++) {
			animation[i].print();
		}*/
	}

	bool active() { return segments != 0; }
	inline void clear()
	{
		segments = 0;loop_ = false;
	}
	sf::IntRect getRect(sf::Time current, int sheet_width) {
		int position = static_cast<int>((ms(current) / animation[index].frame_duration()));
		//cout << "position " << position << endl;
		//cout << "frames " << (int)animation[index].frames << endl;
		//cout << "loops " << (int)animation[index].loops << endl;
		//cout << "multiplied " << (int)animation[index].frames * animation[index].loops << endl;
		if (position >= (int)animation[index].frames * animation[index].loops) {
			//cout << "finished loop" << endl;
			position = 0;
			time_mod = animation[index].length();
			index++;
		}
		if (index == segments) {
			//cout << "index==segments" << endl;
			if (loop_) {
				index = 0;
				time_mod = 0;
			}
			else {
				clear();
				return sf::IntRect(-1, -1, -1, -1);
			}
		}
		position %= animation[index].frames;
		int x = (animation[index].starting.left + animation[index].starting.width * position);
		//auto rect = 
		//cout << "updating rect: " << rect.left << ", " << rect.top << ", " << rect.width << ", " << rect.height << endl;
		return sf::IntRect(x % sheet_width, (animation[index].starting.top + animation[index].starting.height * (x / sheet_width)), animation[index].starting.width, animation[index].starting.height);

	}
};


