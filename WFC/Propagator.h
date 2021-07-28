#pragma once
#include <memory>
#include <cmath>
#include <limits>
#include <string>
#include <stack>
#include <numeric>
#include <optional>
#include "Pattern.h"
#include "Direction.h"
#include "WFCOptions.h"


class Propagator
{
public:
	Propagator(const WFCOptions& options, const std::vector<unsigned>& pattern_weights_, std::pair<std::vector<std::array<std::vector<bool>, 4>>, std::vector<std::array<unsigned, 4>>>&& overlap_rules);

	size_t collapse(std::pair<size_t, size_t> coords);
	std::optional<std::pair<size_t, size_t>> findLowestEntropy();
	/*void setRules(std::vector<std::array<std::vector<bool>, 4>>& rules_,
		std::vector<std::array<unsigned, 4>>& valid_);*/

	auto getWave();
	bool propagate();
	void reset();

private:
	unsigned numpatterns;
	matrix<std::vector<uint8_t>> wave;

	void initEntropy();
	double shannonEntropy(size_t y, size_t x);

	std::vector<unsigned> pattern_weights;
	std::vector<double> pattern_wlogw;
	matrix<double> entropy;
	std::vector<std::array<std::vector<bool>, 4>> rules;
	matrix<std::vector<std::array<unsigned, 4>>> valid;
	std::stack<std::tuple<int, int, size_t>> pstack;

	matrix<uint8_t> collapsed;

	void checkNeighbors(const std::tuple<int, int, size_t> removed);
};


Propagator::Propagator(const WFCOptions& options, const std::vector<unsigned>& pattern_weights_, std::pair<std::vector<std::array<std::vector<bool>, 4>>, std::vector<std::array<unsigned, 4>>>&& overlap_rules) :
	numpatterns(pattern_weights_.size()), pattern_weights(pattern_weights_), wave(options.owidth - options.n + 1, options.oheight - options.n + 1, std::vector<uint8_t>(numpatterns, 1)), pattern_wlogw(), entropy(), rules(std::move(overlap_rules.first)),
	valid(wave.width(), wave.height(), overlap_rules.second), collapsed(wave.width(), wave.height())
{
	uint8_t test = wave.at(0, 0)[0];
	while (test != 1) {

	}
	initEntropy();
	srand((unsigned int)time(NULL));
}

void Propagator::reset() {
	wave.set(wave.width(), wave.height(), std::vector<uint8_t>(numpatterns, 1));
	double max_entropy = shannonEntropy(0, 0);
	entropy.set(wave.width(), wave.height(), max_entropy);
	collapsed.set(wave.width(), wave.height(), 0);
}


auto Propagator::getWave() { return &wave; }


//void Propagator::setRules(std::vector<std::array<std::vector<bool>, 4>>& rules_,
//	std::vector<std::array<unsigned, 4>>& valid_)
//{
//	rules = std::move(rules_);
//	valid.set(wave.size(), wave.height(), valid_);
//}

//init wlogw vector, set entropy of all squares to max value

void Propagator::initEntropy()
{
	for (auto i : pattern_weights)
	{
		pattern_wlogw.push_back(log(i) * i);
	}
	double max_entropy = shannonEntropy(0, 0);
	entropy.set(wave.width(), wave.height(), max_entropy);
}

//find all squares with the lowest current entropy and return one at random

std::optional<std::pair<size_t, size_t>> Propagator::findLowestEntropy()
{
	double lowestEntropy = DBL_MAX;
	std::vector<std::pair<size_t, size_t>> squares = {};
	for (size_t i = 0; i < wave.width(); i++)
	{
		for (size_t j = 0; j < wave.height(); j++)
		{
			if (collapsed.at(i, j))
				continue; //ignore already collapsed squares
			if (entropy.at(i, j) < lowestEntropy)
			{
				squares.clear();
				squares.emplace_back(i, j);
				lowestEntropy = entropy.at(i, j);
			}
			else if (entropy.at(i, j) == lowestEntropy)
			{
				squares.emplace_back(i, j);
			}
		}
	}
	if (squares.size() == 0)
		return std::nullopt;
	return squares[rand() % squares.size()];//return random lowest entropy square of those found
}

//choose a pattern for collapsed square randomly, weighted by frequency

size_t Propagator::collapse(std::pair<size_t, size_t> coords)
{
	auto [y, x] = coords;
	int total = 0;
	for (unsigned i = 0; i < numpatterns; i++)
	{
		if (!wave.at(y, x)[i])
			continue;
		total += pattern_weights[i];
	}
	total = rand() % total;
	for (unsigned i = 0; i < numpatterns; i++)
	{
		if (!wave.at(y, x)[i])
			continue;
		total -= pattern_weights[i];

		if (total < 0)
		{
			for (unsigned j = 0; j < numpatterns; j++)
			{
				if (!wave.at(y, x)[j] || j == i)
					continue;
				wave.at(y, x)[j] = 0;
				pstack.push(std::make_tuple(y, x, j));
			}
			entropy.at(y, x) = 0;
			collapsed.at(y, x) = true;
			return i;
		}
	}
	assert(false); //collapse failed somehow
	return 0;
}


double Propagator::shannonEntropy(size_t y, size_t x)
{
	double sumwlogw = 0, sumweight = 0;
	for (unsigned i = 0;i < numpatterns;i++)
	{
		if (!wave.at(y, x)[i]) continue;
		sumwlogw += pattern_wlogw[i];
		sumweight += pattern_weights[i];
	}
	if (sumweight == 0) return -1; //if a contradiction has occurred, set entropy to -1 to indicate error
	double ret = log(sumweight) - (sumwlogw / sumweight);
	if (ret < .01)
		ret = 0;
	return ret;
}


void Propagator::checkNeighbors(const std::tuple<int, int, size_t> removed)
{
	std::optional<std::pair<int, int>> next;
	int remaining;
	size_t y, x;

	std::vector<unsigned> to_remove;
	std::vector<unsigned> temp;
	for (auto dir : directions) //for each direction from the removed pattern
	{
		if ((next = getCoords(std::get<0>(removed), std::get<1>(removed), wave, dir)).has_value()) //get the adjacent square in the direction
		{
			y = next.value().first;
			x = next.value().second;
			if (entropy.at(y, x) == 0) continue;
			for (unsigned j = 0; j < numpatterns; j++) //for each pattern remaining in the square
			{
				if (!wave.at(y, x)[j])
					continue;
				if (rules[std::get<2>(removed)][dir][j]) //if pattern was compatible with removed pattern
				{
					remaining = --valid.at(y, x)[j][oppositeDir(dir)]; //decrease number of valid patterns in the opposite direction
					if (remaining == 0)
					{
						pstack.push(std::make_tuple(y, x, j)); //then, if allowed patterns is 0, propagate that pattern
						wave.at(y, x)[j] = 0; //and remove it from the wave
					}
				}
			};

			entropy.at(y, x) = shannonEntropy(y, x);
		}
	}
}

//After collapse, eliminate conflicting patterns from overlapping squares, then adjust entropy for those squares
//return false if contradiction has occurred
bool Propagator::propagate()
{
	std::tuple<int, int, size_t> current;
	while (!pstack.empty())
	{
		current = pstack.top();
		pstack.pop();
		checkNeighbors(current);
		if (entropy.at(std::get<0>(current), std::get<1>(current)) == -1)  return false;
	}
	return true;
}
