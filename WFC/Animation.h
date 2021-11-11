#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <array>
#include <iostream>
#include <algorithm>
#include "Debug.h"
using std::cout;
using std::endl;

//start at a given rect, and move right along the sprite sheet (mod width) for a certain number of frames
//More complex animations, like changing directions or jumping during movement, handled by RealTime piecing these together. 
struct Animation {
	sf::IntRect starting;
	sf::Time start_time;
	uint16_t loop_length; //ms for now
	uint16_t pause;
	bool loop;
	uint8_t frames;
	inline float frame_duration() const { return static_cast<float>(loop_length) / frames; }

	sf::IntRect getRect(sf::Time current, int sheet_width);
	bool active() { return start_time != sf::Time(); }
	void print() {
		cout << to_str(starting);
		//cout << "ll: " << static_cast<int>(loop_length) << " frames: " << static_cast<int>(frames) << " pause: " << static_cast<int>(pause) << endl;
	}
	void start(sf::Time time) { start_time = time; }
	Animation() = default;
	inline void setLooping(bool looping) { loop = looping; }
	Animation(const sf::IntRect& start, unsigned short loop_len, uint8_t num_frames, unsigned short pause_after_loop = 0);
};


