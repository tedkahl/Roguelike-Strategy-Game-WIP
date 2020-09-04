#pragma once
#include "Pattern.h"
#include <memory>
#include "WFCOptions.h"

template <typename T>
class Propagator
{
public:
	std::vector< matrix<std::vector<std::shared_ptr<matrix<T> > > > > rules;

	bool allowed(const size_t patternaindex, const matrix<T> &patternb, const int yoffset, const int xoffset);
	void setOverlapRules(std::vector<matrix<T>> &patterns);
private:
	WFCOptions options;

};

/*Intialize rules matrix for each detected pattern. Rules stores, for each possible pattern and each overlap position,
a pointer to each pattern that can overlap in that position. Other patterns are not permitted in that position.*/
template <typename T>
void Propagator<T>::setOverlapRules(std::vector<matrix<T>> &patterns)
{
	int rule_matrix_size = (options.n * 2 - 1);
	for (int i = 0; i < patterns.size(); i++) {
		rules.push_back(matrix<std::vector<std::shared_ptr<matrix<T> > > >);
		for (int j = 0; j < rule_matrix_size; j++) {
			rules[i].push_back({});
			for (int k = 0; k < rule_matrix_size; k++) {
				rules[i][j].push_back({});
				for (auto l : patterns) {
					if (overlaps(i, l, j + 1 - options.n, k + 1 - options.n)) //eg when j and k are 0 and n=3, set overlap rules for -2,-2 position
						rules[i][j][k].push_back(std::make_shared<matrix<T>>(&l));
				}
			}
		}
	}
}


/*Checks if pattern is allowed in given position according to current rules*/
template <typename T>
bool Propagator<T>::allowed(const size_t patternaindex, const matrix<T> &patternb, const int yoffset, const int xoffset) {
	
	for (auto i : rules[patternaindex][yoffset - 1 + options.n][xoffset - 1 + options.n])
		if (*i == patternb) return true;
	return false;
}
