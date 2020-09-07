#pragma once
#include <vector>
#include <assert.h>
#include <algorithm>

template<class T>
using matrix = std::vector<std::vector<T>>;

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


/*Returns submatrix of specified height and width, beginning at given offsets*/
template <typename T>
matrix<T> subMatrix(const matrix<T> &mat, size_t yoffset, size_t xoffset, size_t height, size_t width) {
	assert(yoffset + height <= mat.size());
	assert(xoffset + width <= mat[0].size());
	matrix<T> submatrix(height, std::vector<T>(width));
	for (size_t i = 0; i < height; i++) {
		for (size_t j = 0; j < width; j++) {
			submatrix[i][j] = mat[yoffset + i][xoffset + j];
		}
	}
	return submatrix;
}

/*Determines if two patterns can overlap in a given configuration. yoffset and xoffset specify the position of b 
relative to a*/
template <typename T>
bool overlaps(const matrix<T> &a, const matrix<T> &b, const int yoffset, const int xoffset)
{
	size_t ystarta = std::max(0, yoffset);
	size_t xstarta = std::max(0, xoffset);
	size_t ystartb = std::max(0, -yoffset);
	size_t xstartb = std::max(0, -xoffset);
	size_t overlap_height = a.size() - std::abs(yoffset);
	size_t overlap_width = a.size() - std::abs(xoffset);

	for (size_t i = 0; i < overlap_height; i++) {
		for (size_t j = 0; j < overlap_width; j++) {
			if (a[ystarta + i][xstarta + j] != b[ystartb + i][xstartb + j]) return false;
		}
	}
	return true;
}


/*Returns pattern reflected about the x axis*/
template <typename T>
matrix<T> reflect(matrix<T> &pattern) {
	size_t n = pattern.size();
	matrix<T> reflected(n, std::vector<T>(n));
	for (size_t i = 0; i < n; i++)
		for (size_t j = 0; j < n; j++) {
			reflected[i][n - 1 - j] = pattern[i][j];
		}
	return reflected;

}

/*Returns the matrix rotated 90 degrees clockwise*/
template <typename T>
matrix<T> rotate90(matrix<T> &pattern)
{
	size_t n = pattern.size();
	matrix<T> rotated(n, std::vector<T>(n));
	for (size_t i = 0; i < n; i++)
		for (size_t j = 0; j < n; j++) {
			rotated[j][n - 1 - i]=pattern[i][j];
		}
	return rotated;
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