#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include "Pattern.h"
#include "Tests.h"
const unsigned default_n = 3;
template <class T>
class WFC
{
public:
	void getPatterns();
	void insertRotations(matrix<T> &pattern);
	void initOutput();
	WFC(matrix<T> &input_, size_t oheight, size_t owidth, const unsigned n=default_n, bool rotate=true, bool reflect=true);
private:
	matrix<T> input;
	matrix<T> output;
	std::vector<matrix<T>> patterns;
	std::vector<unsigned> pattern_weights;
	std::unordered_map<matrix<T>, size_t> pmap;

	Propagator<T> propagator;
	unsigned sumofweights;

	WFCOptions options;
};

template <class T>
WFC<T>::WFC(matrix<T> &input_, size_t oheight, size_t owidth, unsigned n, bool rotate, bool reflect)
	: sumofweights(0), input(input_), options(oheight, owidth, n, rotate, reflect)
{
	//input=vector<vector<T>>(owidth,vector<T>(oheight,T()));
	getPatterns();
	propagator.initPropagator(options, patterns, pattern_weights, sumofweights);
	propagator.generate();
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
}

/*template <class T>
void WFC<T>::getInput(std::string fname) {
	//f.open(fname.c_str(), ios::in | ios::out | ios::binary);
	

}*/


template <class T>
void WFC<T>::getPatterns() {
	matrix<T> pattern;
	for (size_t i = 0; i <= input.size() - options.n; i++) {
		for (size_t j = 0; j <= input[0].size() - options.n;j++) {
			pattern = subMatrix(input, i, j, options.n, options.n);
			insertRotations(pattern);
			if (options.reflect) {
				pattern = reflect(pattern);
				insertRotations(pattern);
			}
		}
	}
}

template <class T>
void WFC<T>::insertRotations(matrix<T> &pattern) {
	int rotations = options.rotate ? 4 : 1;
	for (int i = 0; i < rotations; i++)
	{
		auto res = pmap.insert(std::make_pair(pattern, patterns.size()));
		if (res.second) {
			patterns.push_back(pattern);
			pattern_weights.push_back(1);
		}
		else {
			pattern_weights[res.first->second]++;
		}
		sumofweights++;
		pattern = rotate90(pattern);
	}
}

template <class T>
void WFC<T>::initOutput() {
	output(std::vector<T>(options.owidth), options.oheight);
	for (auto i : output) i.push_back(std::make_shared<matrix<T>>(&i));
}