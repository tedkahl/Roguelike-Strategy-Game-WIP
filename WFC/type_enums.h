#pragma once

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