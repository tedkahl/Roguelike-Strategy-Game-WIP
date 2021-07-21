#pragma once
#define NOMINMAX
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <windows.h>
#include <functional>
#include "Pattern.h"
#include "Tests.h"
#include "Direction.h"
template<typename T>
concept hashable = requires(T t) {
	std::hash<T>{}(t);
};

template<typename T>
struct traits;

template<>
struct traits<char> {
	static const char blank = '_';
};

template <typename T>
requires hashable<T>
class WFC
{
public:
	std::vector < matrix<T>> getPatterns();
	void insertRotations(std::vector < matrix<T>>& patternlist, matrix<T>& pattern, std::unordered_map<matrix<T>, size_t>& pmap);
	WFC() = delete;
	WFC(matrix<T>& input_, const size_t oheight, const size_t owidth,
		const unsigned n = 3, const bool rotate = true, const bool reflect = true);
	void displayOutput(size_t y, size_t x);
	std::optional<matrix<T>> step();
	matrix<T> run(bool display = false);
private:
	matrix<T> input;
	matrix<T> output;
	WFCOptions options;
	std::vector<unsigned> pattern_weights;
	std::vector<matrix<T>> patterns;
	Propagator prop;

	void updateOutput(size_t pindex, size_t y, size_t x);
	auto getOverlapRules();
	unsigned sumofweights;

};

//output.assign(options.oheight, std::vector<T>(options.owidth, '_'));

template <class T>
requires hashable<T>
WFC<T>::WFC(matrix<T>& input_, const size_t oheight, const size_t owidth,
	const unsigned n, const bool rotate, const bool reflect)
	: options(oheight, owidth, n, rotate, reflect), sumofweights(0), input(input_), output(oheight, std::vector<T>(owidth, T(traits<T>::blank))),
	patterns(getPatterns()), prop(pattern_weights) {
	auto rules = getOverlapRules();
	prop.setRules(rules.first, rules.second);
}

template <class T>
requires hashable<T>
std::vector<matrix<T>> WFC<T>::getPatterns() {
	matrix<T> pattern;
	std::vector<matrix<T>> patternlist;

	std::unordered_map<matrix<T>, size_t> pmap;
	for (size_t i = 0; i <= input.size() - options.n; i++) {
		for (size_t j = 0; j <= input[0].size() - options.n;j++) {
			pattern = subMatrix(input, i, j, options.n, options.n);
			insertRotations(patternlist, pattern, pmap);
			if (options.reflect) {
				pattern = reflect(pattern);
				insertRotations(patternlist, pattern, pmap);
			}
		}
	}
	return patternlist;
}

template <class T>
requires hashable<T>
void WFC<T>::insertRotations(std::vector<matrix<T>>& patternlist, matrix<T>& pattern,
	std::unordered_map<matrix<T>, size_t>& pmap) {
	int rotations = options.rotate ? 4 : 1;
	for (int i = 0; i < rotations; i++)
	{
		auto res = pmap.insert(std::make_pair(pattern, patternlist.size()));
		if (res.second) {
			patternlist.push_back(pattern);
			pattern_weights.push_back(1);
		}
		else {
			pattern_weights[res.first->second]++;
		}
		sumofweights++;
		pattern = rotate90(pattern);
	}
}
template<typename T>
requires hashable<T>
matrix<T> WFC<T>::run(bool display)
{
	size_t collapsed;
	while (true) {
		auto coords_opt = prop.findLowestEntropy();
		if (!coords_opt) {
			std::cout << "fully collapsed!" << std::endl;
			break; //no square found
		}
		auto& coords = coords_opt.value();
		collapsed = prop.collapse(coords);
		updateOutput(collapsed, coords.first, coords.second);
		if (display)displayOutput(coords.first, coords.second);
		prop.propagate();
	}
	return output;
}

template<typename T>
requires hashable<T>
std::optional<matrix<T>> WFC<T>::step()
{
	auto coords_opt = prop.findLowestEntropy();
	if (!coords_opt) {
		return std::nullopt;
	}
	auto& coords = coords_opt.value();
	size_t collapsed = prop.collapse(coords);
	updateOutput(collapsed, coords.first, coords.second);
	prop.propagate();
	return output;
}


template<typename T>
requires hashable<T>
void WFC<T>::displayOutput(size_t y, size_t x) {
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	for (size_t i = 0; i < output.size(); i++) {
		for (size_t j = 0; j < output[0].size(); j++) {
			if (i >= y && i < y + options.n &&
				j >= x && j < x + options.n) {
				SetConsoleTextAttribute(hConsole, 4);
				std::cout << output[i][j];
				SetConsoleTextAttribute(hConsole, 7);
			}
			else std::cout << output[i][j];
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

/*Intialize rules matrix for each detected pattern. Rules stores, for each possible pattern and each overlap position,
the index of each pattern that can overlap in that position. Other patterns are not permitted in that position.*/

template<typename T>
requires hashable<T>
auto WFC<T>::getOverlapRules()
{
	std::vector<bool> empty(patterns.size());
	std::vector<std::array<std::vector< bool >, 4 > > rules(patterns.size(),
		std::array<std::vector<bool>, 4>({ empty,empty,empty,empty }));
	std::vector<std::array<unsigned, 4>> valid(patterns.size());

	for (size_t i = 0; i < patterns.size(); i++) {
		for (auto j : directions) {
			for (size_t k = 0; k < patterns.size(); k++) {
				if (overlaps(patterns[i], patterns[k], ydir[j], xdir[j])) {
					rules[i][j][k] = 1; //pattern l is allowed in this overlap position with i
					valid[i][j]++;
				}
			}
		}
	}
	return std::make_pair(rules, valid);
}

template <typename T>
requires hashable<T>
void WFC<T>::updateOutput(size_t pindex, size_t y, size_t x) {
	for (size_t i = 0; i < options.n; i++) {
		for (size_t j = 0; j < options.n; j++) {

			output[y + i][x + j] = patterns[pindex][i][j];
		}
	}
}
