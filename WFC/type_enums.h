#pragma once

enum class terrain_type {
	GRASS, WATER, PAVEDSTONE, PAVEDSTONE_TALL, ROUGHSTONE, LAVA, SAND, TERRAIN_END
};
//status effects here temporarily
enum class object_type {
	NONE, MOVESELECT, ATTACKSELECT, DEFAULTSELECT,
	POISON, PETRIFY, SLOW,
	ARROW,
	ROCK, CACTUS,
	DUELIST, WOLF, OBJECT_END
};

enum class move_type {
	WALK, FLY, MOVE_END
};

enum class attack_type {
	NO_ATTACK, MELEE, RANGED_LOS, RANGED_ARROW
};

enum class anim_state {
	DEFAULT, WALKING, ATTACKING, DEFENDING, DYING, ANIM_STATE_END
};

enum obj_height { SQUARE, FLAT, TALL };
constexpr int firstproj = static_cast<int>(object_type::ARROW);
constexpr int firstunit = static_cast<int>(object_type::DUELIST);
constexpr int firstobj = static_cast<int>(object_type::ROCK);
static constexpr int unitIndex(object_type u) {
	return static_cast<int>(u) - firstunit;
}
static constexpr bool isUnit(object_type t) {
	return static_cast<int>(t) >= firstunit;
}
static constexpr bool isUnitOrObj(object_type t) {
	return static_cast<int>(t) >= firstobj;
}
static constexpr bool isObj(object_type t) {
	return isUnitOrObj(t) && !isUnit(t);
}

static constexpr int getDefaultOrder(object_type t) {
	if (t >= object_type::ROCK) return 50;
	else if (t >= object_type::ARROW) return 76;
	else if (t >= object_type::POISON) return 51 - static_cast<int>(object_type::POISON) + static_cast<int>(t);
	else return 1;
}