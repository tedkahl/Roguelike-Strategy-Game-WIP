#pragma once
#include "Animation.h"
#include "type_enums.h"
constexpr int num_units = static_cast<int>(object_type::OBJECT_END) - firstunit;
const int sprite_dim = 72;
constexpr int x = unitIndex(object_type::DUELIST);


struct simple_unit_anim
{
private:
	struct anim_info
	{
		//row that animation starts at (for now must start at the beginning of a row)
		uint8_t row;
		//frames in animation
		uint8_t frames;
	};
	bool is_set = false;
	std::array<anim_info, static_cast<int>(anim_state::ANIM_STATE_END)> anim_rows_frames = {};
public:
	void set(std::array<anim_info, static_cast<int>(anim_state::ANIM_STATE_END)> data)
	{
		anim_rows_frames = data;
		is_set = true;
	}
	anim_info& get(anim_state state) {
		return anim_rows_frames[static_cast<int>(state)];
	}
};

//this is not how it's going to work. 
class animation_manager {
private:
	animation_manager() {
		//DEFAULT MOVE ATTACK DEFEND DIE
		anims[unitIndex(object_type::DUELIST)].set({ 0, 1,  2, 8,  1, 1,  0, 1,  2, 8 });
		anims[unitIndex(object_type::WOLF)].set({ 0, 1, 0, 1, 0, 1, 0, 1, 0, 1 });
	}
	inline static std::unique_ptr<animation_manager> instance_;
	std::array<simple_unit_anim, static_cast<int>(object_type::OBJECT_END) - firstunit> anims;
public:

	static animation_manager* instance() {
		if (!instance_) instance_ = std::unique_ptr<animation_manager>(new animation_manager());
		return instance_.get();
	}
	int get_loop_length(object_type otype, anim_state state) {
		auto& info = anims[unitIndex(otype)].get(state);
		return std::max(150 * info.frames, 500);
	}


	Animation get_basic_anim(object_type otype, anim_state state, unsigned short loop_length = 0) {
		auto& info = anims[unitIndex(otype)].get(state);
		cout << "row: " << (int)info.row << " frames " << (int)info.frames << endl;
		if (loop_length == 0)
			loop_length = get_loop_length(otype, state);

		return Animation(sf::IntRect(0, sprite_dim * info.row, sprite_dim, sprite_dim), loop_length, info.frames);
	}
};