#pragma once
#include "Pattern.h"
#include "Propagator.h"
#include "WFCOptions.h"

template<typename T>
void printPattern(matrix<T> &p) {
	for (auto i : p) {
		for (auto j : i) std::cout << j;
		std::cout << std::endl;
	}
}

template <typename T>
void printOverlaps(const size_t index, const Propagator<T> &prop, const WFCOptions options)
{
	for (int i = 0; i < 2*options.n-1; i++) {
		for (int j = 0; j < 2*options.n-1; j++) {
			std::cout << i - options.n << ", " << j - options.n << ":" << std::endl;
			for (auto k : prop.rules[index][i][j]) {
				printPattern(*k);
				std::cout << std::endl;
			}
		}
	}
}