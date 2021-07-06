#pragma once
#include "Pattern.h"
#include "Propagator.h"
#include "WFCOptions.h"

/*std::copy_if(adjoining.begin(), adjoining.end(), std::back_inserter(tmp), [&, this](size_t pindex) {
				if (rules[std::get<2>(removed)][i][pindex]) { //if pattern was compatible with removed pattern
					remaining = --valid[y][x][pindex][oppositeDir(i)]; //decrease number of valid patterns for that pattern
					if (remaining == 0) { //then, if allowed patterns is 0, propagate that pattern
						pstack.push(std::make_tuple(y, x, pindex));
						return false; //and remove it from the wave
					}
				}
				return true;
			});*/

//printOverlaps(60, propagator, patterns, options);
/*for (size_t i = 0; i < patterns.size();i++) {
	printPattern(patterns[i]);
	std::cout << "Weight: "<< pattern_weights[i]<<std::endl<<std::endl;
}*/

/*for (size_t i = 0; i < patterns.size(); i++) {
	printPattern(patterns[i]);
	printOverlaps<T>(i, propagator, patterns, options);
	std::cout << "----------------------------------------------------------------------------" << std::endl;

}*/

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
void printWave(std::vector<matrix<T>>& patterns, Propagator& prop) {
	auto* wave = prop.getWave();
	for (unsigned i = 0;i < wave->size();i++) for (unsigned j = 0;j < (*wave)[0].size();j++) {
		std::cout << "at " << i << ", " << j << std::endl;
		for (unsigned k = 0; k < (*wave)[i][j].size();k++) {
			if ((*wave)[i][j][k]) std::cout << k << ",";
		}
		std::cout << std::endl;
	}
}

template<typename T>
void printPattern(const matrix<T> &p) {
	for (auto i : p) {
		for (auto j : i) std::cout << j;
		std::cout << std::endl;
	}
}

template<typename T>
void printValidPatterns(std::pair<size_t, size_t> coords, size_t collapsed, std::vector<matrix<T>>& patterns, Propagator& prop) {
	auto* wave = prop.getWave();
	auto [y, x] = coords;
	std::cout << "y " << y << " x " << x << std::endl;
	printPattern(patterns[collapsed]);
	for (auto dir : directions) {
		if (y + ydir[dir] < 0 || (unsigned)y + ydir[dir] >= wave->size() ||
			x + xdir[dir] < 0 || (unsigned)x + xdir[dir] >= (*wave)[0].size()) continue;

		std::cout << "valid in dir " << dir << std::endl;
		for (int i = 0; i < patterns.size();i++) {
			if (*wave[y + ydir[dir]][x + xdir[dir]][i]) {
				printPattern(patterns[i]);
				std::cout << std::endl;
			}
		}
	}
}


/*
template <typename T>
void printOverlaps(const size_t index, const Propagator &prop, const std::vector<matrix<T>> &patterns, const WFCOptions options)
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
}*/