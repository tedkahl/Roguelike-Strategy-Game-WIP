#pragma once
#include <memory>
#include <cmath>
#include <limits>
#include <string>
#include <stack>
#include <optional>
#include <windows.h>
#include "Pattern.h"
#include "Direction.h"
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

	void initPropagator(const WFCOptions &op, std::vector<matrix<T>> &patterns, 
		std::vector<unsigned> &pattern_weights_, const int sumofweights_);
	
	std::vector<size_t>& waveAt(std::pair<int, int> coords);
	Propagator() {};
	
private:
	WFCOptions options;
	int sumofweights; 

	void initEntropy();
	void initWave();
	void initOverlapRules();
	double shannonEntropy(size_t y, size_t x) const;

	size_t collapse(size_t y, size_t x);
	std::pair<size_t, size_t> findLowestEntropy();

	void updateOutput(const size_t pindex, const size_t y, const size_t x);

	std::vector<matrix<T>> patterns;
	std::vector<unsigned> pattern_weights;
	std::vector<double> pattern_wlogw;

	void getNeighbors(std::stack<std::pair<int, int>> &flagged, const std::pair<int, int> coords);
	void propagate(const size_t pindex, const std::pair<int, int> coords);
	bool checkConflicts(const std::pair<int, int> coords);

	matrix<std::vector<size_t>> wave;
	matrix<T> output;
	matrix<double> entropy;
};

//Functions as a constructor
template <typename T>
void Propagator<T>::initPropagator(const WFCOptions &options_, std::vector<matrix<T>> &patterns_,
	std::vector<unsigned> &pattern_weights_, const int sumofweights_) {
	srand((unsigned int)time(NULL));

	options = options_;
	patterns = patterns_;
	pattern_weights = pattern_weights_;
	sumofweights = sumofweights_;

	output.assign(options.oheight, std::vector<T>(options.owidth, '_'));

	initWave();
	initEntropy();
	initOverlapRules();

}


//init wlogw vector, set entropy of all squares to max value
template <typename T>
void Propagator<T>::initEntropy() {
	for (auto i : pattern_weights) {
		pattern_wlogw.push_back(log(i) * i);
	}	
	double max_entropy = shannonEntropy(0, 0);
	entropy.assign(wave.size(), std::vector<double>(wave[0].size(), max_entropy));
}

//Set size of wave and initialize with all possible patterns
template <typename T>
void Propagator<T>::initWave(){
	std::vector<size_t> allpatterns(patterns.size());
	for (size_t i = 0; i < patterns.size(); i++) allpatterns[i] = i;

	wave.assign(options.oheight - options.n + 1,
		std::vector<std::vector<size_t>>(options.owidth - options.n + 1, allpatterns));

}


/*Intialize rules matrix for each detected pattern. Rules stores, for each possible pattern and each overlap position,
the index of each pattern that can overlap in that position. Other patterns are not permitted in that position.*/
template <typename T>
void Propagator<T>::initOverlapRules()
{
	rules.assign(patterns.size(), matrix<std::vector<bool>>(options.n * 2 - 1,
		std::vector<std::vector<bool>>(options.n * 2 - 1, std::vector<bool>(patterns.size()))));
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
	while (true) {
		coords = findLowestEntropy();
		if (coords.first == wave.size()) break; //no square found
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

		propagate(collapsed, coords);
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

//find all squares with the lowest current entropy and return one at random
template<typename T>
std::pair<size_t, size_t> Propagator<T>::findLowestEntropy()
{
	double lowestEntropy = DBL_MAX;
	std::vector<std::pair<size_t, size_t>> squares = {};
	for (size_t i = 0; i < wave.size(); i++) {
		for (size_t j = 0; j < wave[0].size(); j++) {
			if (entropy[i][j] == 0) continue; //ignore already collapsed squares
			if (entropy[i][j] < lowestEntropy) {
				squares.clear();
				squares.emplace_back(i, j);
				lowestEntropy = entropy[i][j];
			}
			else if (entropy[i][j] == lowestEntropy) {
				squares.push_back(std::make_pair(i, j));
			}
		}
	}
	if (squares.size() == 0) return std::make_pair(wave.size(), wave.size()); //indicates fully collapsed
	return squares[rand() % squares.size()]; //return random lowest entropy square of those found
}

//choose a pattern for collapsed square randomly, weighted by frequency
template<typename T>
size_t Propagator<T>::collapse(size_t y, size_t x) {
	int total = 0;
	for (auto i:wave[y][x]) total += pattern_weights[i];
	total = rand() % total;
	for (auto i:wave[y][x]) {
		total -= pattern_weights[i];

		if (total < 0) { 
			wave[y][x] = { i };
			entropy[y][x] = 0;
			return i;
		}
	}
	assert(false); //collapse failed somehow
}

template <typename T>
double Propagator<T>::shannonEntropy(size_t y, size_t x) const
{
	double sumwlogw = 0, sumweight=0;
	for (auto i:wave[y][x]) {
			sumwlogw += pattern_wlogw[i];
			sumweight += pattern_weights[i];
		}
	return log(sumweight) - (sumwlogw / sumweight);
}

template <typename T>
std::vector<size_t>& Propagator<T>::waveAt(std::pair<int, int> coords) {
	return wave[coords.first][coords.second];
}

template <typename T>
void Propagator<T>::getNeighbors(std::stack<std::pair<int,int>> &flagged, const std::pair<int, int> coords)
{
	std::optional<std::pair<int, int>> next;

	for (auto i:directions){
		if((next = getCoords(coords, wave, i)).has_value() && entropy[next.value().first][next.value().second] != 0)
		flagged.push(next.value());
	}
}

//After collapse, eliminate conflicting patterns from overlapping squares, then adjust entropy for those squares
template <typename T>
void Propagator<T>::propagate(size_t pindex, const std::pair<int,int> coords)
{
	std::cout << "propagate pattern " << pindex << "at " << coords.first << ", " << coords.second << std::endl;
	
	std::stack<std::pair<int, int>> flagged;
	std::pair<int, int> current;
	getNeighbors(flagged, coords);

	bool definite;
	while (!flagged.empty()) {
		//std::cout << flagged.size() << std::endl;
		current = flagged.top();
		flagged.pop();
		definite = checkConflicts(current);
		//std::cout << (changed? "Changed" : "Not changed");

		if (definite) {
			getNeighbors(flagged, current);
		}
	}	
}

//Checks for conflicts with adjacent definite patterns at a given square, removes invalid patterns from wave
//returns true if sqyare is now definite (has only one valid pattern)
template <typename T>
bool Propagator<T>::checkConflicts(const std::pair<int,int> coords)
{
	int yindex, xindex;
	std::vector<size_t> temp;

	std::optional<std::pair<int, int>> next;

	for (auto i : directions) {
		if ((next = getCoords(coords, wave, i)).has_value() && entropy[next.value().first][next.value().second] == 0) {
			yindex = coords.first - next.value().first + (int)options.n - 1;
			xindex = coords.second - next.value().second + (int)options.n - 1;
			std::copy_if(waveAt(coords).begin(), waveAt(coords).end(), std::back_inserter(temp), [=, this](size_t pindex) {
				return rules[waveAt(next.value())[0]][yindex][xindex][pindex];
				});
			waveAt(coords) = std::move(temp);
			entropy[coords.first][coords.second] = shannonEntropy(coords.first, coords.second);
		}
	}
		
	
	if (waveAt(coords).size() == 0) {
		std::cout << "Contradiction at " << coords.first << ", " << coords.second;
		assert(false);
	}

	return temp.size()==1;
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
