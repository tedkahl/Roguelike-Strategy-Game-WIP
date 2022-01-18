#pragma once
#include "Animation.h"
#include "Data.h"
#include "type_enums.h"


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

static sf::IntRect getStartingRect(object_type otype, uint8_t row) {
	auto search = Data<char>::d()->entityinfo.find(otype);
	if (search != Data<char>::d()->entityinfo.end()) {
		auto rect{ std::get<2>(search->second) };
		rect.top += rect.height * row;
		return rect;
	}
	std::cerr << "object not found" << endl;
	assert(false);

}
//this is not how it's going to work. 
class animation_manager {
private:
	animation_manager() {
		//DEFAULT MOVE ATTACK DEFEND DIE
		anims[static_cast<int>(object_type::DUELIST)].set({ 0, 1,  2, 8,  1, 1,  0, 1,  2, 8 });
		anims[static_cast<int>(object_type::WOLF)].set({ 0, 1, 0, 1, 0, 1, 0, 1, 0, 1 });
		anims[static_cast<int>(object_type::ARROW)].set({ 0, 1, 0, 3, 0, 1, 0, 1, 0, 1 });
	}
	inline static std::unique_ptr<animation_manager> instance_;
	std::array<simple_unit_anim, static_cast<int>(object_type::OBJECT_END)> anims;
public:

	static animation_manager* instance() {
		if (!instance_) instance_ = std::unique_ptr<animation_manager>(new animation_manager());
		return instance_.get();
	}
	int get_loop_length(object_type otype, anim_state state) {
		auto& info = anims[static_cast<int>(otype)].get(state);
		return std::max(150 * info.frames, 500);
	}


	Animation get_basic_anim(object_type otype, anim_state state, unsigned short loop_length = 0, bool reverse = false) {

		auto& info = anims[static_cast<int>(otype)].get(state);
		cout << "row: " << (int)info.row << " frames " << (int)info.frames << endl;
		if (loop_length == 0)
			loop_length = get_loop_length(otype, state);
		auto r = getStartingRect(otype, info.row);
		if (reverse) {
			r.left = r.left + r.width;
			r.width = -r.width;
		}
		return Animation(r, loop_length, info.frames);
	}
};