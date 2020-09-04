#pragma once
#include "Pattern.h"


template<typename T>
void printPattern(Pattern<T> &p) {
	for (auto i : p.matrix_) {
		for (auto j : i) std::cout << j;
		std::cout << std::endl;
	}
}

template <typename T>
void printOverlaps(const Pattern<T> &p)
{
	int n = p.matrix_.size();
	for (int i = 0; i < p.rules.size(); i++) {
		for (int j = 0; j < p.rules[0].size(); j++) {
			std::cout << i - n << ", " << j - n << ":" << std::endl;
			for (auto k : p.rules[i][j]) {
				printPattern(*k);
				std::cout << std::endl;
			}
		}
	}
}