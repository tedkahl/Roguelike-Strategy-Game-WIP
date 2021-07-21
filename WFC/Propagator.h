#pragma once
#include <memory>
#include <cmath>
#include <limits>
#include <string>template<size_t H, size_t W>
#include <stack>
#include <numeric>
#include <optional>
#include "Pattern.h"
#include "Direction.h"
#include "WFCOptions.h"


class Propagator
{
public:
	Propagator(const std::vector<unsigned>& pattern_weights_);

	std::vector<std::array<std::vector<bool>, 4>> rules;
	matrix<std::vector<std::array<unsigned, 4>>> valid;

	size_t collapse(std::pair<size_t, size_t> coords);
	std::optional<std::pair<size_t, size_t>> findLowestEntropy();
	void setRules(std::vector<std::array<std::vector<bool>, 4>>& rules_,
		std::vector<std::array<unsigned, 4>>& valid_);

	auto getWave();
	void propagate();

private:
	unsigned numpatterns;
	matrix<std::vector<unsigned>> wave;

	void initEntropy();
	double shannonEntropy(size_t y, size_t x) const;

	std::vector<unsigned> pattern_weights;
	std::vector<double> pattern_wlogw;
	matrix<double> entropy;
	std::stack<std::tuple<int, int, size_t>> pstack;

	matrix<bool> collapsed;

	void checkNeighbors(const std::tuple<int, int, size_t> removed);
};


Propagator::Propagator(const std::vector<unsigned>& pattern_weights_) : numpatterns(pattern_weights_.size()), pattern_weights(pattern_weights_),
pattern_wlogw(), entropy()
{
	std::vector<unsigned> p_indices(numpatterns);
	std::iota(p_indices.begin(), p_indices.end(), 0);
	for (auto i : wave) for (auto j : i) j = p_indices;
	initEntropy();
	srand((unsigned int)time(NULL));
}

auto Propagator::getWave() { return &wave; }


void Propagator::setRules(std::vector<std::array<std::vector<bool>, 4>>& rules_,
	std::vector<std::array<unsigned, 4>>& valid_)
{
	rules = std::move(rules_);
	valid.assign(wave.size(), std::vector<std::vector<std::array<unsigned, 4>>>(wave[0].size(), valid_));
}

//init wlogw vector, set entropy of all squares to max value

void Propagator::initEntropy()
{
	for (auto i : pattern_weights)
	{
		pattern_wlogw.push_back(log(i) * i);
	}
	double max_entropy = shannonEntropy(0, 0);
	entropy.assign(wave.size(), std::vector<double>(wave[0].size(), max_entropy));
}

//find all squares with the lowest current entropy and return one at random

std::optional<std::pair<size_t, size_t>> Propagator::findLowestEntropy()
{
	double lowestEntropy = DBL_MAX;
	std::vector<std::pair<size_t, size_t>> squares = {};
	for (size_t i = 0; i < wave.size(); i++)
	{
		for (size_t j = 0; j < wave[0].size(); j++)
		{
			if (collapsed[i][j])
				continue; //ignore already collapsed squares
			if (entropy[i][j] < lowestEntropy)
			{
				squares.clear();
				squares.emplace_back(i, j);
				lowestEntropy = entropy[i][j];
			}
			else if (entropy[i][j] == lowestEntropy)
			{
				squares.emplace_back(i, j);
			}
		}
	}
	if (squares.size() == 0)
		return std::nullopt;
	return squares[rand() % squares.size()];					 //return random lowest entropy square of those found
}

//choose a pattern for collapsed square randomly, weighted by frequency

size_t Propagator::collapse(std::pair<size_t, size_t> coords)
{
	auto [y, x] = coords;
	int total = 0;
	for (auto i : wave[y][x])
	{
		total += pattern_weights[i];
	}
	total = rand() % total;
	for (auto i : wave[y][x])
	{
		total -= pattern_weights[i];

		if (total < 0)
		{
			wave[y][x] = { i };
			pstack.push(std::make_tuple(y, x, i));
			entropy[y][x] = 0;
			collapsed[y][x] = true;
			return i;
		}
	}
	assert(false); //collapse failed somehow
	return 0;
}


double Propagator::shannonEntropy(size_t y, size_t x) const
{
	double sumwlogw = 0, sumweight = 0;
	for (unsigned i = 0; i < numpatterns; i++)
	{
		if (!wave[y][x][i])
			continue;
		sumwlogw += pattern_wlogw[i];
		sumweight += pattern_weights[i];
	}
	assert(sumweight != 0); //check for contradiction
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
	for (auto dir : directions)
	{
		if ((next = getCoords(std::get<0>(removed), std::get<1>(removed), wave.size(), dir)).has_value())
		{
			y = next.value().first;
			x = next.value().second;
			if (collapsed[y][x]) continue;
			temp = {};
			to_remove = {};
			for (auto j : wave[y][x])
			{
				if (rules[std::get<2>(removed)][dir][j])
				{												  //if pattern was compatible with removed pattern
					remaining = --valid[y][x][j][oppositeDir(dir)]; //decrease number of valid patterns for that pattern
					if (remaining == 0)
					{ //then, if allowed patterns is 0, propagate that pattern
						pstack.push(std::make_tuple(y, x, j));
						to_remove.push_back(j); //and prepare to remove it from the wave
					}
				}
			};

			//remove invalid patterns from the wave
			std::copy_if(wave[y][x].begin(), wave[y][x].end(), std::back_inserter(temp), [&](unsigned val) {return !std::binary_search(to_remove.begin(), to_remove.end(), val);});
			wave[y][x] = std::move(temp);
			entropy[y][x] = shannonEntropy(y, x);
		}
	}
}

//After collapse, eliminate conflicting patterns from overlapping squares, then adjust entropy for those squares

void Propagator::propagate()
{
	std::tuple<int, int, size_t> current;
	while (!pstack.empty())
	{
		current = pstack.top();
		pstack.pop();
		checkNeighbors(current);
	}
}
