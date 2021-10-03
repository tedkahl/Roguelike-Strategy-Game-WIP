#pragma once
#include "Animation.h"
#include "type_enums.h"
constexpr int num_units = object_type::OBJECT_END - firstunit;
const int sprite_dim = 72;
constexpr int x = unitIndex(object_type::DUELIST);

const int S = 3;
class animation_manager {
private:
	animation_manager() {
		//DEFAULT MOVE ATTACK DEFEND DIE
		anims[unitIndex(object_type::DUELIST)].set({ 0, 1,  2, 8,  1, 1,  0, 1,  2, 8 });
		anims[unitIndex(object_type::WOLF)].set({ 0, 1, 0, 1, 0, 1, 0, 1, 0, 1 });
	}
	inline static std::unique_ptr<animation_manager> instance_;
	struct simple_unit_anim
	{
	private:
		struct anim_info
		{
			uint8_t row;
			uint8_t frames;
		};
		bool is_set = false;
		std::array<anim_info, ANIM_STATE_END> anim_rows_frames;
	public:
		void set(std::array<anim_info, ANIM_STATE_END> data)
		{
			anim_rows_frames = data;
			is_set = true;
		}
		anim_info& get(anim_state state) {
			return anim_rows_frames[state];
		}
	};
	std::array<simple_unit_anim, OBJECT_END - firstunit> anims;
public:

	static animation_manager* instance() {
		if (!instance_) instance_ = std::unique_ptr<animation_manager>(new animation_manager());
		return instance_.get();
	}
	int get_loop_length(object_type otype, anim_state state) {
		auto& info = anims[unitIndex(otype)].get(state);
		return std::max(150 * info.frames, 500);
	}

	//std::vector<AnimationSeg> get_anim()
	std::vector<AnimationSeg> get_pause_anim(sf::IntRect rect, int duration) {
		return std::vector<AnimationSeg>(1, AnimationSeg(rect, duration, 1, 1));
	}
	std::vector<AnimationSeg> get_basic_anim(object_type otype, anim_state state, uint8_t loops, int loop_length = 0) {
		auto& info = anims[unitIndex(otype)].get(state);
		cout << "row: " << (int)info.row << " frames " << (int)info.frames << endl;
		if (loop_length == 0)
			loop_length = get_loop_length(otype, state);
		AnimationSeg myseg(sf::IntRect(0, sprite_dim * info.row, sprite_dim, sprite_dim), loop_length, info.frames, loops);
		cout << to_str(myseg.starting) << ", " << myseg.loop_length << ", " << (int)myseg.frames << ", " << (int)myseg.loops << endl;
		return std::vector<AnimationSeg>(1, std::move(myseg));
	}
};