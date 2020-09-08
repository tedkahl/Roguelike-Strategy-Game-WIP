#pragma once
#include <memory>
#include <cmath>
#include <limits>
#include <string>
#include <windows.h>
#include "Pattern.h"
#include "WFCOptions.h"


template <typename T>
class Propagator
{
public:
	//test
	void printOverlaps(const size_t index);
	void printPattern(const size_t index);

	std::vector<matrix<std::vector< bool > > >  rules;	
	void generate();
	
	bool allowed(const size_t patternaindex, const size_t patternbindex, const int yoffset, const int xoffset);

	void initPropagator(const WFCOptions &op, std::vector<matrix<T>> &patterns, 
		std::vector<unsigned> &pattern_weights_, const int sumofweights_);
	
	Propagator() {};
	
private:
	size_t rule_matrix_size;
	WFCOptions options;
	int sumofweights; 

	void initWLogW();
	void initEntropy();
	void initWave();
	void initOverlapRules();
	double shannonEntropy(size_t y, size_t x) const;

	size_t collapse(size_t y, size_t x);
	bool fully_collapsed() const;
	std::pair<size_t, size_t> findLowestEntropy();

	void updateOutput(const size_t pindex, const size_t y, const size_t x);

	std::vector<matrix<T>> patterns;
	std::vector<unsigned> pattern_weights;
	std::vector<double> pattern_wlogw;

	void getNeighbors(const int y, const int x);
	void propagate(const size_t pindex, const int y, const int x);

	matrix<std::vector<bool>> wave;
	matrix<T> output;
	matrix<double> entropy;
	matrix<unsigned> num_patterns;
};

//Functions as a constructor
template <typename T>
void Propagator<T>::initPropagator(const WFCOptions &options_, std::vector<matrix<T>> &patterns_,
	std::vector<unsigned> &pattern_weights_, const int sumofweights_) {
	srand((unsigned int)time(NULL));

	options = options_;
	rule_matrix_size = options.n * 2 - 1;
	patterns = patterns_;
	pattern_weights = pattern_weights_;
	sumofweights = sumofweights_;

	output = *new matrix<T>(options.oheight, std::vector<T>(options.owidth, '_'));

	initWave();
	initEntropy();
	initOverlapRules();

}

template <typename T>
void Propagator<T>::initWLogW() {
	for (auto i : pattern_weights) {
		pattern_wlogw.push_back(log(i) * i);
	}
}

//init wlogw vector, set entropy of all squares to max value
template <typename T>
void Propagator<T>::initEntropy() {
	initWLogW();
	double max_entropy = shannonEntropy(0, 0);
	entropy = *new matrix<double>(wave.size(), std::vector<double>(wave[0].size(), max_entropy));
}

//Set size of wave and initialize with all possible patterns
template <typename T>
void Propagator<T>::initWave()
{
	std::vector<bool> allpatterns(patterns.size(), true);
	
	wave = *new matrix<std::vector<bool>>(options.oheight - options.n + 1,
		std::vector<std::vector<bool>>(options.owidth - options.n + 1, allpatterns));

	num_patterns = *new matrix<unsigned>(wave.size(), std::vector<unsigned>(wave[0].size(), patterns.size()));
}


/*Intialize rules matrix for each detected pattern. Rules stores, for each possible pattern and each overlap position,
the index of each pattern that can overlap in that position. Other patterns are not permitted in that position.*/
template <typename T>
void Propagator<T>::initOverlapRules()
{
	rules = *new std::vector < matrix<std::vector<bool>>>(patterns.size(), matrix<std::vector<bool>>(rule_matrix_size,
		std::vector<std::vector<bool>>(rule_matrix_size, std::vector<bool>(patterns.size()))));
	int n = options.n;
	for (size_t i = 0; i < patterns.size(); i++) {
		for (int j = 1 - n; j <= n - 1; j++) {
			for (int k = 1 - n; k <= n - 1; k++) { //so for n=3, loop from -2,-2 to 2,2
				for (size_t l = 0; l < patterns.size(); l++) {
					if (overlaps(patterns[i], patterns[l], j, k))
						rules[i][j + n - 1][k + n - 1][l] = 1; //pattern l is allowed in this overlap position with i
				}
			}
		}
	}
}

template <typename T>
void Propagator<T>::generate()
{
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 7);
	
	std::pair<size_t, size_t> coords;
	size_t collapsed;
	while (!fully_collapsed()) {
		coords = findLowestEntropy();
		collapsed = collapse(coords.first, coords.second);
		updateOutput(collapsed, coords.first, coords.second);

		for (size_t i = 0; i < output.size(); i++) {
			for (size_t j = 0; j < output[0].size(); j++) {
				if (i >= coords.first&&i < coords.first + options.n &&
					j >= coords.second&&j < coords.second + options.n) {
					SetConsoleTextAttribute(hConsole, 4);
					std::cout << output[i][j];
					SetConsoleTextAttribute(hConsole, 7);
				}
				else std::cout << output[i][j];
			}
			std::cout << std::endl;
		}

		propagate(collapsed, coords.first, coords.second);
	}

}


template<typename T>
void Propagator<T>::updateOutput(size_t pindex, size_t y, size_t x) {
	for (size_t i = 0; i < options.n; i++) {
		for (size_t j = 0; j < options.n; j++) {
			output[y + i][x + j] = patterns[pindex][i][j];
		}
	}
}
//true if all squares in wave are collapsed
template<typename T>
bool Propagator<T>::fully_collapsed() const {
	for (auto i : entropy) for (auto j : i) if (j != -1) return false;
	return true;
}

//find all squares with the lowest current entropy and return one at random
template<typename T>
std::pair<size_t, size_t> Propagator<T>::findLowestEntropy()
{
	double lowestEntropy = DBL_MAX;
	std::vector<std::pair<size_t, size_t>> squares = {};
	for (size_t i = 0; i < wave.size(); i++) {
		for (size_t j = 0; j < wave[0].size(); j++) {
			if (entropy[i][j] == -1) continue; //ignore already collapsed squares
			if (entropy[i][j] < lowestEntropy) {
				squares.clear();
				squares.push_back(std::make_pair(i, j));
				lowestEntropy = entropy[i][j];
			}
			else if (entropy[i][j] == lowestEntropy) {
				squares.push_back(std::make_pair(i, j));
			}
		}
	}
	return squares[rand() % squares.size()]; //return random lowest entropy square of those found
}

//choose a pattern for collapsed square randomly, weighted by frequency
template<typename T>
size_t Propagator<T>::collapse(size_t y, size_t x) {
	int total = 0;
	for (size_t i = 0; i < patterns.size();i++) if(wave[y][x][i]) total += pattern_weights[i];
	total = rand() % total;
	for (size_t i = 0; i < patterns.size(); i++) {
		if(wave[y][x][i]) total -= pattern_weights[i];

		if (total < 0) { //collapse to this pattern
			//wave[y][x].clear();
			//wave[y][x][i] = true;
			entropy[y][x] = -1;
			return i;
		}
	}
	assert(false); //collapse failed somehow
}

template <typename T>
double Propagator<T>::shannonEntropy(size_t y, size_t x) const
{
	double sumwlogw = 0, sumweight=0;
	for (size_t i = 0; i < patterns.size(); i++) {
		if(wave[y][x][i])
		{
			sumwlogw += pattern_wlogw[i];
			sumweight += pattern_weights[i];
		}
	}
	return log(sumweight) - (sumwlogw / sumweight);
}

template <typename T>
void Propagator<T>::getNeighbors(const int y, const int x)
{
	int  ystart = std::max(0, y - (int)options.n + 1);
	int  yend = std::min((int)wave.size(), y + (int)options.n);
	int  xstart = std::max(0, x - (int)options.n + 1);
	int  xend = std::min((int)wave[0].size(), x + (int)options.n);

	for (int i = ystart; i < yend; i++) {
		for (int j = xstart; j < xend; j++) {
			if (entropy[i][j] == -1) continue;
			stack.push(std::make_pair(i, j));
		}
	}
}

//After collapse, eliminate conflicting patterns from overlapping squares, then adjust entropy for those squares
template <typename T>
void Propagator<T>::propagate(size_t pindex, int y, int x)
{
	std::cout << "propagate pattern " << pindex << "at " << y << ", " << x << std::endl;
	
	std::pair<int, int> current;
	getNeighbors(y, x);

	bool changed;
	while (!stack.empty()) {
		std::cout << stack.size() << std::endl;
		current = stack.top();
		stack.pop();
		changed = checkConflicts(current.first, current.second);
		std::cout << (changed? "Changed" : "Not changed");

		if (wave[current.first][current.second] == empty) {
			std::cout << "Contradiction at " << current.first << ", " << current.second;
			assert(false);
		}

		if (changed) {
			entropy[current.first][current.second] = shannonEntropy(current.first, current.second);
			getNeighbors(current.first, current.second);
		}
	}	
}

/*Checks if pattern is allowed in given position according to current rules*/
template <typename T>
bool Propagator<T>::allowed(const size_t patternaindex, const size_t patternbindex, const int yoffset, const int xoffset) {
	return rules[patternaindex][yoffset + (int)options.n - 1][xoffset + (int)options.n - 1][patternbindex];
}

//test
template<typename T>
void Propagator<T>::printPattern(const size_t index)
{
	int count = 0;
	for (auto i : patterns[index]) {
		std::cout << count % 10;
		for (auto j : i) {
			std::cout << j;
		}
		count++;
		std::cout << std::endl;
	}
}


template<typename T>
void Propagator<T>::printOverlaps(const size_t index)
{
	for (int i = 0; i < 2 * (int)options.n - 1; i++) {
		for (int j = 0; j < 2 * (int)options.n - 1; j++) {
			std::cout << i - (int)options.n + 1 << ", " << j - (int)options.n + 1 << ":" << std::endl;
			for (auto k : rules[index][i][j]) {
				printPattern(k);
				std::cout << std::endl;
			}
		}
	}
}
