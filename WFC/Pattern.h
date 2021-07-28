#pragma once
#include <assert.h>
#include <algorithm>
#include <vector>
#include "matrix.h"


/*Returns submatrix of specified width and height, beginning at given offsets*/
template <typename T>
matrix<T> subMatrix(matrix<T>& mat, const size_t yoffset, const size_t xoffset, const size_t width, const size_t height) {
	assert(yoffset + width <= mat.width());
	assert(xoffset + height <= mat.height());
	matrix<T> submatrix(width, height);
	for (size_t i = 0; i < width; i++) {
		for (size_t j = 0; j < height; j++) {
			submatrix.at(i, j) = mat.at(yoffset + i, xoffset + j);
		}
	}
	return submatrix;
}

/*Determines if two patterns can overlap in a given configuration. yoffset and xoffset specify the position of b
relative to a*/
template <typename T>
bool overlaps(const matrix<T>& a, const matrix<T>& b, const int yoffset, const int xoffset)
{
	size_t ystarta = std::max(0, yoffset),
		xstarta = std::max(0, xoffset),
		ystartb = std::max(0, -yoffset),
		xstartb = std::max(0, -xoffset),
		overlap_width = a.width() - std::abs(yoffset),
		overlap_height = a.height() - std::abs(xoffset);

	for (size_t i = 0; i < overlap_width; i++) {
		for (size_t j = 0; j < overlap_height; j++) {
			if (a.at(ystarta + i, xstarta + j) != b.at(ystartb + i, xstartb + j)) return false;
		}
	}
	return true;
}


/*Returns pattern reflected about the x axis*/
template <typename T>
matrix<T> reflect(const matrix<T>& pattern) {
	size_t n = pattern.width();
	matrix<T> reflected(n, n);
	for (size_t i = 0; i < n; i++)
		for (size_t j = 0; j < n; j++) {
			reflected.at(i, n - 1 - j) = pattern.at(i, j);
		}
	return reflected;

}

/*Returns the matrix rotated 90 degrees clockwise*/
template <typename T>
matrix<T> rotate90(const matrix<T>& pattern)
{
	size_t n = pattern.width();
	matrix<T> rotated(n, n);
	for (size_t i = 0; i < n; i++)
		for (size_t j = 0; j < n; j++) {
			rotated.at(j, n - 1 - i) = pattern.at(i, j);
		}
	return rotated;
}
