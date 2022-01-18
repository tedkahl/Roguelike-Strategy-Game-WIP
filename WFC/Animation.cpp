#include "Animation.h"
sf::IntRect Animation::getRect(sf::Time current, int sheet_width) {
	auto ms = (current - start_time).asMilliseconds();
	//if first loop is over, deactivate if animation is not set to loop. Else pause before repeating.
	if (ms >= loop_length) {
		if (!loop) {
			start_time = sf::Time();
		}
		ms %= (loop_length + pause);
		return { -1,-1,-1,-1 };
	}
	uint16_t position = static_cast<int>(ms / frame_duration());
	position %= frames;
	int x = (starting.left + position * abs(starting.width));
	return sf::IntRect((x == sheet_width && starting.width < 0) ? sheet_width : x % sheet_width, (starting.top + starting.height * (x / sheet_width)), starting.width, starting.height);
}
Animation::Animation(const sf::IntRect& start, unsigned short loop_len, uint8_t num_frames, unsigned short pause_after_loop) :starting(start), start_time(), loop_length(loop_len), frames(num_frames), pause(pause_after_loop), loop(false) {}