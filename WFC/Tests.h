#pragma once
#include "Pattern.h"
#include "Propagator.h"
#include "WFCOptions.h"


//test: pattern overlaps with self
/*for (int i = 0; i < patterns.size();i++) {
	printPattern(patterns[i]);
	std::cout << std::endl;
	std::cout << rules[i][1][1].size() << std::endl;
	printPattern(patterns[rules[i][1][1][0]]);
	std::cout << std::endl;
	std::cout << "Allowed? " << allowed(i, patterns[i], 0, 0) << std::endl;
}*/

template<typename T>
void printPattern(const matrix<T> &p) {
	for (auto i : p) {
		for (auto j : i) std::cout << j;
		std::cout << std::endl;
	}
}

template <typename T>
void printOverlaps(const size_t index, const Propagator<T> &prop, const std::vector<matrix<T>> &patterns, const WFCOptions options)
{
	for (int i = 0; i < 2 * (int)options.n - 1; i++) {
		for (int j = 0; j < 2 * (int)options.n - 1; j++) {
			std::cout << i - (int)options.n + 1 << ", " << j - (int)options.n + 1 << ":" << std::endl;
			for (auto k : prop.rules[index][i][j]) {
				printPattern(patterns[k]);
				std::cout << std::endl;
			}
		}
	}
}