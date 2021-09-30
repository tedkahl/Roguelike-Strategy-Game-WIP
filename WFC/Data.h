#pragma once
#include <map>
#include <memory>
#include <string>
#include <SFML/Graphics.hpp>
#include "Animation.h"
namespace sq {
	const static float square_w = 80.f;
	const static float square_h = 40.f;
	const static float square_t = 15.f;
}
template<typename T>
struct Data;

enum terrain_type {
	GRASS, WATER, PAVEDSTONE, PAVEDSTONE_TALL, ROUGHSTONE, LAVA, SAND, TERRAIN_END
};

enum object_type {
	NONE, MOVESELECT, ATTACKSELECT,
	ROCK, CACTUS,
	DUELIST, WOLF, OBJECT_END
};

enum move_type {
	WALK, FLY, MOVE_END
};

enum attack_type {
	NO_ATTACK, MELEE, RANGED_LOS, RANGED_ARROW
};

enum anim_state {
	DEFAULT, WALKING, ATTACKING, DEFENDING, DYING, ANIM_STATE_END
};

enum obj_height { SQUARE, FLAT, TALL };
const int firstunit = object_type::DUELIST;
static constexpr int unitIndex(object_type u) {
	return u - firstunit;
}
static constexpr bool isUnit(object_type t) {
	return t >= firstunit;
}
static constexpr bool isTall(object_type t) {
	return t > object_type::MOVESELECT;
}

//
//Animation* get_animation(object_type otype, anim_state state) {
//	switch (otype) {
//	case: object_type::DUELIST{
//		switch (state) {
//		case: anim_state::ATTACK{
//			return smooth(sf::Clock., int duration, int num_frames)
//		}
//}
//	}
//	}
//
//}


template<typename T>
struct Data {
private:
	Data();
	inline static std::unique_ptr<Data<T>> instance_;
public:
	static Data<T>* d() {
		if (!instance_) instance_ = std::unique_ptr<Data<T>>(new Data<T>());
		return instance_.get();
	}
	~Data() = default;
	Data(const Data<T>&) = delete;
	Data& operator= (const Data<T>) = delete;
	std::map < T, std::tuple<terrain_type, object_type>> glyphs;
	std::map <unsigned, std::tuple<std::string, sf::Vector2f, sf::IntRect>> squareinfo; //path, offset, texture rect
	std::map <unsigned, std::tuple<std::string, sf::Vector2f, sf::IntRect>> entityinfo;
	std::array<std::array<int, terrain_type::TERRAIN_END>, move_type::MOVE_END> movecosts;




};

template<typename T>
void combine_vectors(std::vector<T>& first, std::vector<std::vector<T>>&& vecs) {
	for (auto&& i : vecs) {
		first.reserve(first.size() + i.size());
		first.insert(first.end(), std::make_move_iterator(i.begin()), std::make_move_iterator(i.end()));
	}
}

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

template<typename T>
Data<T>::Data()
{
	movecosts[move_type::FLY].fill(1);
	movecosts[move_type::WALK] = { 1,2,1,1,1,999,2 };



	glyphs.emplace(std::make_pair('.', std::make_tuple(terrain_type::LAVA, object_type::NONE)));
	glyphs.emplace(std::make_pair('x', std::make_tuple(terrain_type::PAVEDSTONE, object_type::NONE)));
	glyphs.emplace(std::make_pair('C', std::make_tuple(terrain_type::PAVEDSTONE, object_type::ROCK)));
	glyphs.emplace(std::make_pair('T', std::make_tuple(terrain_type::PAVEDSTONE_TALL, object_type::NONE)));
	glyphs.emplace(std::make_pair('R', std::make_tuple(terrain_type::PAVEDSTONE_TALL, object_type::ROCK)));
	glyphs.emplace(std::make_pair('w', std::make_tuple(terrain_type::WATER, object_type::NONE)));
	glyphs.emplace(std::make_pair('g', std::make_tuple(terrain_type::GRASS, object_type::NONE)));
	glyphs.emplace(std::make_pair('G', std::make_tuple(terrain_type::GRASS, object_type::ROCK)));
	glyphs.emplace(std::make_pair('s', std::make_tuple(terrain_type::SAND, object_type::CACTUS)));

	squareinfo.emplace(std::make_pair(terrain_type::LAVA, std::make_tuple("./textures/lava0.png", sf::Vector2f(0, 10), sf::IntRect())));
	squareinfo.emplace(std::make_pair(terrain_type::PAVEDSTONE, std::make_tuple("./textures/whitepaved0.png", sf::Vector2f(), sf::IntRect())));
	squareinfo.emplace(std::make_pair(terrain_type::PAVEDSTONE_TALL, std::make_tuple("./textures/whitepavedtall.png", sf::Vector2f(0, -15), sf::IntRect())));
	squareinfo.emplace(std::make_pair(terrain_type::WATER, std::make_tuple("./textures/lightwater0.png", sf::Vector2f(), sf::IntRect())));
	squareinfo.emplace(std::make_pair(terrain_type::GRASS, std::make_tuple("./textures/grass0.png", sf::Vector2f(), sf::IntRect())));
	squareinfo.emplace(std::make_pair(terrain_type::SAND, std::make_tuple("./textures/sand0.png", sf::Vector2f(), sf::IntRect())));

	entityinfo.emplace(std::make_pair(object_type::ROCK, std::make_tuple("./textures/brownrock0.png", sf::Vector2f(9, -40), sf::IntRect())));
	entityinfo.emplace(std::make_pair(object_type::CACTUS, std::make_tuple("./textures/orig.png", sf::Vector2f(22, -40), sf::IntRect(211, 1579, 44, 66))));
	entityinfo.emplace(std::make_pair(object_type::MOVESELECT, std::make_tuple("./textures/moveselect.png", sf::Vector2f(), sf::IntRect())));
	entityinfo.emplace(std::make_pair(object_type::ATTACKSELECT, std::make_tuple("./textures/attackselect.png", sf::Vector2f(), sf::IntRect())));
	entityinfo.emplace(std::make_pair(object_type::DUELIST, std::make_tuple("./textures/duelist_sheet.png", sf::Vector2f(6, -33), sf::IntRect(0, 0, 72, 72))));
	entityinfo.emplace(std::make_pair(object_type::WOLF, std::make_tuple("./textures/direwolf.png", sf::Vector2f(6, -33), sf::IntRect())));
}