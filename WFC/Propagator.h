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

	std::vector<matrix<std::vector< size_t > > >  rules;	
	void generate();
	
	bool allowed(const size_t patternaindex, const size_t patternbindex, const int yoffset, const int xoffset);

	void initPropagator(const WFCOptions &op, std::vector<matrix<T>> &patterns, 
		std::vector<unsigned> &pattern_weights_, const int sumofweights_);
	
	void propagate(const int y, const int x);
	Propagator() {};
	
private:
	size_t rule_matrix_size;
	WFCOptions options;
	int sumofweights; 

	void initWLogW();
	void initEntropy();
	void initWave();
	void setOverlapRules();
	double shannonEntropy(size_t y, size_t x) const;

	void collapse(size_t y, size_t x);
	bool fully_collapsed() const;
	std::pair<size_t, size_t> findLowestEntropy();

	void updateOutput(size_t y, size_t x);

	std::vector<matrix<T>> patterns;
	std::vector<unsigned> pattern_weights;
	std::vector<double> pattern_wlogw;

	matrix<std::vector<size_t>> wave;
	matrix<T> output;
	matrix<double> entropy;
};

template<typename T>
void Propagator<T>::updateOutput(size_t y, size_t x) {
	for (size_t i = 0; i < options.n; i++) {
		for (size_t j = 0; j < options.n; j++) {
			output[y + i][x + j] = patterns[wave[y][x][0]][i][j];
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
void Propagator<T>::collapse(size_t y, size_t x) {
	int total = 0;
	for (auto i : wave[y][x]) total += pattern_weights[i];
	total = rand() % total;
	for (auto i : wave[y][x]) {
		total -= pattern_weights[i];
		if (total < 0) { //collapse to this pattern
			wave[y][x] = { i };
			entropy[y][x] = -1;
			return;
		}
	}
	assert(false); //collapse failed somehow
}

template <typename T>
void Propagator<T>::initWLogW(){
	for (auto i : pattern_weights) {
		pattern_wlogw.push_back(log(i) * i);
	}
}

template <typename T>
double Propagator<T>::shannonEntropy(size_t y, size_t x) const
{
	double sumwlogw = 0, sumweight=0;
	for (auto i : wave[y][x]) {
		sumwlogw += pattern_wlogw[i];
		sumweight += pattern_weights[i];
	}
	return log(sumweight) - (sumwlogw / sumweight);
}

template <typename T>
void Propagator<T>::initEntropy() {
	initWLogW();
	double max_entropy = shannonEntropy(0, 0);
	entropy = *new matrix<double>(wave.size(), std::vector<double>(wave[0].size(), max_entropy));
}


//Functions as a constructor
template <typename T>
void Propagator<T>::initPropagator(const WFCOptions &options_, std::vector<matrix<T>> &patterns_,
	std::vector<unsigned> &pattern_weights_, const int sumofweights_) {
	srand(time(NULL));

	options = options_;
	rule_matrix_size = options.n * 2 - 1;
	patterns = patterns_;
	pattern_weights = pattern_weights_;
	sumofweights = sumofweights_;

	output = *new matrix<T>(options.oheight, std::vector<T>(options.owidth, '_'));

	initWave();
	initEntropy();
	setOverlapRules();

}

template<typename T>
void printPattern(matrix<T> &pattern)
{
	int count = 0;
	for (auto i : pattern) {
		std::cout << count%10;
		for (auto j : i) {
			std::cout<<j;
		}
		count++;
		std::cout << std::endl;
	}
}

template <typename T>
void Propagator<T>::generate()
{
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 7);

	while (!fully_collapsed()) {
		auto coords = findLowestEntropy();
		collapse(coords.first, coords.second);
		updateOutput(coords.first, coords.second);
		//std::cout << std::string(20, '\n');
		

		propagate(coords.first, coords.second);
	}
	for (int i = 0; i < output.size(); i++) {
		for (int j = 0; j < output[0].size(); j++) {
			/*if (i >= coords.first&&i < coords.first + options.n &&
				j >= coords.second&&j < coords.second + options.n) {
				SetConsoleTextAttribute(hConsole, 4);
				std::cout << output[i][j];
				SetConsoleTextAttribute(hConsole, 7);
			}*/
			std::cout << output[i][j];
		}
		std::cout << std::endl;
	}
}

//After collapse, eliminate conflicting patterns from overlapping squares, then adjust entropy for those squares
template <typename T>
void Propagator<T>::propagate(int y, int x)
{
	std::cout << "propagate pattern " << wave[x][y][0] << "at " << y << ", " << x << std::endl;
	printPattern(patterns[wave[x][y][0]]);
	int n = options.n;	

	size_t ystart = std::max(0, y - n + 1);
	size_t yend = std::min((int)wave.size(), y + n );
	size_t xstart = std::max(0, x - n + 1);
	size_t xend = std::min((int)wave[0].size(), x + n );

	for (size_t i = ystart; i < yend; i++) {
		for (size_t j = xstart; j < xend; j++) {
			if (wave[i][j].size() <= 1) continue;
			
			//test print
			/*std::cout << i << ", " << j << ", " << std::endl;
			for (auto it : wave[i][j]) {
				std::cout << it << std::endl; printPattern(patterns[it]);
			}*/

			std::vector<size_t> temp;
			std::copy_if(wave[i][j].begin(), wave[i][j].end(), std::back_inserter(temp),
				[=,this](size_t pindex) {return allowed(wave[y][x][0], pindex, (int)i-(int)y, (int)j-(int)x); });
			wave[i][j] = std::move(temp);
			
			if (wave[i][j].size() == 0) {
				std::cout << "Contradiction at " << i << ", " << j;
				assert(wave[i][j].size() != 0); //contradiction, throw an exception or something
			}

			//test print
			/*printPattern(patterns[wave[y][x][0]]);
			std::cout << i << ", " << j << ", " << std::endl;
			for (auto it : wave[i][j]) { std::cout << it << std::endl; printPattern(patterns[it]);  }
			*/
			entropy[i][j] = shannonEntropy(i, j);
			//std::cout << "Entropy: " << entropy[i][j] << std::endl;
			
		}
	}
}

//Set size of wave and initialize with all possible patterns
template <typename T>
void Propagator<T>::initWave()
{
	std::vector<size_t> allpatterns;
	for (size_t i = 0; i < patterns.size(); i++) allpatterns.push_back(i);
	wave = *new matrix<std::vector<size_t>>(options.oheight - options.n + 1, 
		std::vector<std::vector<size_t>>(options.owidth - options.n + 1, allpatterns));
}


/*Intialize rules matrix for each detected pattern. Rules stores, for each possible pattern and each overlap position,
the index of each pattern that can overlap in that position. Other patterns are not permitted in that position.*/
template <typename T>
void Propagator<T>::setOverlapRules()
{
	rules = *new std::vector < matrix<std::vector<size_t>>>(patterns.size(), matrix<std::vector<size_t>>(rule_matrix_size,
		std::vector<std::vector<size_t>>(rule_matrix_size, std::vector<size_t>())));
	int n = options.n;
	for (size_t i = 0; i < patterns.size();i++) {
		for (int j = 1 - n; j <= n - 1; j++) {
			for (int k = 1 - n; k <= n - 1; k++) { //so for n=3, loop from -2,-2 to 2,2
				for (size_t l = 0; l < patterns.size(); l++) {
					if (overlaps(patterns[i], patterns[l], j, k)) 
						rules[i][j + n - 1][k + n - 1].push_back(l); //pattern l is allowed in this overlap position with i
				}
			}
		}
	}
}

template<typename T>
void Propagator<T>::printOverlaps(const size_t index)
{
	for (int i = 0; i < 2 * (int)options.n - 1; i++) {
		for (int j = 0; j < 2 * (int)options.n - 1; j++) {
			std::cout << i - (int)options.n + 1 << ", " << j - (int)options.n + 1 << ":" << std::endl;
			for (auto k : rules[index][i][j]) {
				printPattern(patterns[k]);
				std::cout << std::endl;
			}
		}
	}
}

/*Checks if pattern is allowed in given position according to current rules*/
template <typename T>
bool Propagator<T>::allowed(const size_t patternaindex, const size_t patternbindex, const int yoffset, const int xoffset) {
	for(auto i:rules[patternaindex][yoffset + (int)options.n - 1][xoffset + (int)options.n - 1]){
			if (i == patternbindex) return true;
			if (i > patternbindex) return false;
	}
	return false;
}
