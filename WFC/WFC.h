#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include "Pattern.h"
#include "Wave.h"
#include "Tests.h"
const unsigned default_n = 3;
template <class T>
class WFC
{
private:
	matrix<T> input;
	matrix<T> output;
	std::vector<Pattern<T>> patterns;
	std::unordered_map<matrix<T>, size_t> pmap;
	//Wave<T> wave;
	WFCOptions options;
public:
	void getPatterns();
	void insertRotations(Pattern<T> &pattern);
	void initOutput();
	WFC(matrix<T> &input_, size_t oheight, size_t owidth, const unsigned n=default_n, bool rotate=true, bool reflect=true);
};

template <class T>
WFC<T>::WFC(matrix<T> &input_, size_t oheight, size_t owidth, unsigned n, bool rotate, bool reflect)
	: input(input_), options(oheight, owidth, n, rotate, reflect)
{
	//input=vector<vector<T>>(owidth,vector<T>(oheight,T()));

	getPatterns();

	setOverlapRules(patterns);
	for (auto i : patterns) printOverlaps(i);
}

/*template <class T>
void WFC<T>::getInput(std::string fname) {
	//f.open(fname.c_str(), ios::in | ios::out | ios::binary);
	

}*/


template <class T>
void WFC<T>::getPatterns() {
	Pattern<T> pattern;
	for (size_t i = 0; i <= input.size() - options.n; i++) {
		for (size_t j = 0; j <= input[0].size() - options.n;j++) {
			pattern.matrix_ = subMatrix(input, i, j, options.n, options.n);
			insertRotations(pattern);
			if (options.reflect) {
				pattern = reflect(pattern);
				insertRotations(pattern);
			}
		}
	}
}

template <class T>
void WFC<T>::insertRotations(Pattern<T> &pattern) {
	int rotations = options.rotate ? 4 : 1;
	for (int i = 0; i < rotations; i++)
	{
		auto res = pmap.insert(std::make_pair(pattern.matrix_, patterns.size()));
		if (res.second) {
			patterns.push_back(pattern);
		}
		else {
			patterns[res.first->second].weight++;
		}
		pattern = rotate90(pattern);
	}
}

template <class T>
void WFC<T>::initOutput() {
	output(std::vector<int>(options.owidth), options.oheight);
	for (auto i : output) i.push_back(std::make_shared<std::vector < std::vector<T>>>(&i));
}