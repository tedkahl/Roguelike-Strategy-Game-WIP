#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <array>
#include <iostream>
#include <algorithm>
#include "Debug.h"
using std::cout;
using std::endl;

const int max_seg = 1;

//start at a given rect, and move right along the sprite sheet (mod width) for a certain number of frames
//More complex animations, like changing directions or jumping during movement, handled by RealTime piecing these together. 
struct Animation {
	sf::IntRect starting;
	uint16_t start_time;
	uint16_t loop_length; //ms for now
	uint16_t pause;
	bool loop;
	uint8_t frames;
	inline float frame_duration() const { return static_cast<float>(loop_length) / frames; }

	sf::IntRect getRect(sf::Time current, int sheet_width) {
		//get our current frame
		uint16_t position = static_cast<int>((current.asMilliseconds() % (loop_length + pause) - start_time) / frame_duration());
		//if frame is greater than max frames, deactivate if loop is not set. Else pause.
		if (position >= frames) {
			if (!loop)
				frames = 0;
			return { -1,-1,-1,-1 };
		}
		int x = (starting.left + position * starting.width);
		return sf::IntRect(x % sheet_width, (starting.top + starting.height * (x / sheet_width)), starting.width, starting.height);
	}
	bool active() { return frames != 0; }
	void print() {
		cout << to_str(starting);
		//cout << "ll: " << static_cast<int>(loop_length) << " frames: " << static_cast<int>(frames) << " pause: " << static_cast<int>(pause) << endl;
	}
	void start(sf::Time time) { start_time = time.asMilliseconds() % (loop_length + pause); }
	Animation() = default;
	inline void setLooping(bool looping) { loop = looping; }
	Animation(const sf::IntRect& start, unsigned short loop_len, uint8_t num_frames, uint8_t num_loops = 1, unsigned short pause_after_loop = 0) :starting(start), start_time(0), loop_length(loop_len), frames(num_frames), pause(pause_after_loop), loop(false) {}
};


