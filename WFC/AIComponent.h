#pragma once
#include "AIPreference.h"

//enum class preference_type {
//	//enemy target preferences
//	ANY_ENEMY, LOW_HP_ENEMY,
//	//example map objective
//	BREAK_ROCK,
//	//example ally objective
//	LOW_HP_ALLY
//};

//later possibly: add state machines that change preferences
//if so, one vector is one state, must add updates etc
class AIComponent
{
public:
	std::vector<preference> state;
	AIComponent(std::initializer_list<preference> init);
};

class EvilMonster :public AIComponent {
	EvilMonster();
};

class SimpleMonster :public AIComponent {
	SimpleMonster();
};

class Healer :public AIComponent {
	Healer();
};