#pragma once
#include <assert.h>
#include <algorithm>
#include <vector>
#include "matrix.h"


/*Returns submatrix of specified width and height, beginning at given offsets*/
template <typename T>
static matrix<T> subMatrix(matrix<T>& mat, size_t xoffset, size_t yoffset, size_t width, size_t height) {
	assert(xoffset + width <= mat.width());
	assert(yoffset + height <= mat.height());
	matrix<T> submatrix(width, height);
	for (size_t y = 0; y < height; y++) {
		for (size_t x = 0; x < width; x++) {
			submatrix.at(x, y) = mat.at(xoffset + x, yoffset + y);
		}
	}
	return submatrix;
}

/*Determines if two patterns can overlap in a given configuration. yoffset and xoffset specify the position of b
relative to a*/
template <typename T>
static bool overlaps(const matrix<T>& a, const matrix<T>& b, int xoffset, int yoffset)
{
	size_t ystarta = std::max(0, yoffset),
		xstarta = std::max(0, xoffset),
		ystartb = std::max(0, -yoffset),
		xstartb = std::max(0, -xoffset),
		overlap_width = a.width() - std::abs(xoffset),
		overlap_height = a.height() - std::abs(yoffset);

	for (size_t i = 0; i < overlap_width; i++) {
		for (size_t j = 0; j < overlap_height; j++) {
			if (a.at(xstarta + i, ystarta + j) != b.at(xstartb + i, ystartb + j)) return false;
		}
	}
	return true;
}


/*Returns pattern reflected about the x axis*/
template <typename T>
static matrix<T> reflect(const matrix<T>& pattern) {
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
static matrix<T> rotate90(const matrix<T>& pattern)
{
	size_t n = pattern.width();
	matrix<T> rotated(n, n);
	for (size_t i = 0; i < n; i++)
		for (size_t j = 0; j < n; j++) {
			rotated.at(j, n - 1 - i) = pattern.at(i, j);
		}
	return rotated;
}
