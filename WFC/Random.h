#pragma once
#include <stdlib.h>
int roll(int numdice, int sides) {
	int sum = 0;
	for (int i = 0;i < numdice;i++) sum += (rand() % sides + 1);
	return sum;
}

struct dice {
	unsigned numdice_, sides_;
	unsigned operator()() const { return roll(numdice_, sides_); }
	dice(unsigned numdice, unsigned sides) :numdice_(numdice), sides_(sides) {}
};