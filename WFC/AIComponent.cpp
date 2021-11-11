#include "AIComponent.h"
AIComponent::AIComponent(std::initializer_list<preference> init) :state(init) {}
EvilMonster::EvilMonster() : AIComponent({ preference(preference_type::LOW_HP_ENEMY) }) {}
SimpleMonster::SimpleMonster() : AIComponent({ preference{preference_type::ANY_ENEMY}, preference{preference_type::BREAK_ROCK, -.1f, true} }) {}
Healer::Healer() : AIComponent({ preference(preference_type::LOW_HP_ALLY, .6f), preference{preference_type::ANY_ENEMY} }) {}