#pragma once
#include <vector>
#include <assert.h>
#include <algorithm>

template<class T>
using matrix = std::vector<std::vector<T>>;

template <typename T>
struct Pattern
{
public:
	matrix<T> matrix_;
	unsigned weight=1; //frequency of pattern in input image

	bool allowed(const Pattern<T> &pattern, int yoffset, int xoffset);

	Pattern();
	Pattern(std::initializer_list<std::initializer_list<T>> &&in);
	Pattern(const matrix<T> &&other);
	Pattern(const matrix<T> &other);


	matrix<std::vector<std::shared_ptr<Pattern<T> > > > rules;

	bool operator== (const Pattern& other);
private: //stores "overlap rules"

};

template <typename T>
bool Pattern<T>::operator==(const Pattern& other) {
	return matrix_ == other.matrix_;
}

template <typename T>
Pattern<T>::Pattern() {}

namespace std {
	template <typename T>
	class hash<matrix<T>> {
	public:
		size_t operator()(const matrix<T>& m) const noexcept{
			size_t seed = sizeof m;
			for (auto i:m) for (auto j:i) {
					seed ^= static_cast<size_t>(j) + 0x9e3779b9 + (seed << 6) + (seed >> 2); //based on Boost's hash_combine
			
			}
			return seed;
		}
	};
}


/*template <typename T>
Pattern<T>& Pattern<T>::operator=(const matrix<T> &&other) {
	matrix = other;
	other = nullptr;
	return *this;
}*/

template <typename T>
Pattern<T>::Pattern(std::initializer_list<std::initializer_list<T>> &&in)
{
	for (auto i : in) matrix_.push_back(i);
}

template <typename T>
Pattern<T>::Pattern(const matrix<T> &&other)
{
	matrix_ = other;
}

template <typename T>
Pattern<T>::Pattern(const matrix<T> &other)
{
	matrix_ = other;
}

/*Checks if pattern is allowed in given position according to current rules*/
template <typename T>
bool Pattern<T>::allowed(const Pattern<T> &pattern, int yoffset, int xoffset) {
	for (auto i : allowed[yoffset - 1 + pattern.size()][xoffset - 1 + pattern.size()])
		if (*i.matrix_ == pattern.matrix_) return true;
	return false;
}

/*Intialize rules matrix for each detected pattern. Rules stores, for each possible overlap position, a pointer to
each pattern that can overlap in that position. Other patterns are not permitted in that position.*/
template <typename T>
void setOverlapRules(std::vector<Pattern<T>> &patterns)
{
	size_t n = patterns[0].matrix_.size();
	int rule_matrix_size = (n * 2 - 1);
	for (auto i : patterns) {
		for (int j = 0; j < rule_matrix_size; j++) {
			i.rules.push_back({});
			for (int k = 0; k < rule_matrix_size; k++) {
				i.rules[j].push_back({});
				for (auto l : patterns) {
					if (overlaps(i, l, j + 1 - n, k + 1 - n))
						i.rules[j][k].push_back(std::make_shared<Pattern<T>>(&l));
				}
			}
		}
	}
}


/*Determines if two patterns can overlap in a given configuration. yoffset and xoffset specify the position of b 
relative to a*/
template <typename T>
bool overlaps(const Pattern<T> &a, const Pattern<T> &b, int yoffset, int xoffset)
{
	size_t n = a.size();
	size_t overlap_height = n - std::abs(yoffset);
	size_t overlap_width = n - std::abs(xoffset);
	assert(std::abs(xoffset) < n);
	assert(std::abs(yoffset) < n);

	return (subMatrix(a.matrix_, std::max(0, yoffset), std::max(0, xoffset), overlap_height, overlap_width) ==
		subMatrix(b.matrix_, std::max(0, -yoffset), std::max(0, -xoffset), overlap_height, overlap_width));
}

/*Returns submatrix of specified height and width, beginning at given offsets*/
template <typename T>
matrix<T> subMatrix(const matrix<T> &mat, size_t yoffset, size_t xoffset, size_t height, size_t width) {
	assert(yoffset + height <= mat.size());
	assert(xoffset + width <= mat[0].size());
	matrix<T> submatrix;
	for (auto i = mat.begin() + yoffset; i != mat.begin() + yoffset + height;i++) {
		submatrix.push_back(std::vector<T>(i->begin() + xoffset, i->begin() + xoffset + width));
	}
	return submatrix;
}

/*Returns pattern reflected about the x axis*/
template <typename T>
Pattern<T> reflect(Pattern<T> &pattern) {
	size_t n = pattern.matrix_.size();
	matrix<T> reflected(n, std::vector<T>(n));
	for (size_t i = 0; i < n; i++)
		for (size_t j = 0; j < n; j++) {
			reflected[i][n - 1 - j] = pattern.matrix_[i][j];
		}
	return Pattern<T>(reflected);

}

/*Returns the matrix rotated 90 degrees clockwise*/
template <typename T>
Pattern<T> rotate90(Pattern<T> &pattern)
{
	matrix<T> rotated(pattern.matrix_[0].size(),std::vector<T>(pattern.matrix_.size()));
	for (size_t i = 0; i < pattern.matrix_.size(); i++)
		for (size_t j = 0; j < pattern.matrix_[0].size(); j++) {
			rotated[j][rotated[0].size()-1-i]=pattern.matrix_[i][j];
		}
	return Pattern<T>(rotated);
}

/*
template <typename T>
matrix<T> rotate180(matrix<T> matrix)
{
	matrix<T> rotated(matrix.size(), std::vector<T>(matrix[0].size()));
	for (size_t i = 0; i < matrix.size(); i++)
		for (size_t j = 0; j < matrix[0].size(); j++) {
			rotated[rotated.size()-1-i][rotated[0].size() -1 - j]= matrix[i][j];
		}
	return rotated;
}

template <typename T>
matrix<T> rotate270(matrix<T> matrix)
{
	matrix<T> rotated(matrix[0].size(), std::vector<T>(matrix.size()));
	for (size_t i = 0; i < matrix.size(); i++)
		for (size_t j = 0; j < matrix[0].size(); j++) {
			rotated[rotated.size() - 1 - j][i]= matrix[i][j];
		}
	return rotated;
}*/