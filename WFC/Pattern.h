#pragma once
#include <assert.h>
#include <algorithm>
#include <vector>
#include <array>
template<typename T, size_t N, size_t M>
using matrix = std::array<std::array<T, M>, N>;

namespace std {
	template<typename T, unsigned N, unsigned M>
	class hash<matrix<T, N, M>> {
	public:
		size_t operator()(const matrix<T, N, M>& m) const noexcept {
			size_t seed = sizeof m;
			for (auto i : m) for (auto j : i) {
				seed ^= std::hash<T>{}(j)+0x9e3779b9 + (seed << 6) + (seed >> 2); //based on Boost's hash_combine

			}
			return seed;
		}
	};
}

/*Returns submatrix of specified height and width, beginning at given offsets*/
template<typename T, unsigned N, unsigned M>
auto subMatrix(const matrix<T, N, M>& mat, const size_t yoffset, const size_t xoffset, const size_t height, const size_t width) {
	assert(yoffset + height <= mat.size());
	assert(xoffset + width <= mat[0].size());
	matrix<T, height, width> submatrix;
	for (size_t i = 0; i < height; i++) {
		for (size_t j = 0; j < width; j++) {
			submatrix[i][j] = mat[yoffset + i][xoffset + j];
		}
	}
	return submatrix;
}

/*Determines if two patterns can overlap in a given configuration. yoffset and xoffset specify the position of b
relative to a*/
template<typename T, unsigned N, unsigned M>
bool overlaps(const matrix<T, N, M>& a, const matrix<T, N, M>& b, const int yoffset, const int xoffset)
{
	unsigned ystarta = std::max(0, yoffset),
		xstarta = std::max(0, xoffset),
		ystartb = std::max(0, -yoffset),
		xstartb = std::max(0, -xoffset),
		overlap_height = a.size() - std::abs(yoffset),
		overlap_width = a.size() - std::abs(xoffset);

	for (unsigned i = 0; i < overlap_height; i++) {
		for (unsigned j = 0; j < overlap_width; j++) {
			if (a[ystarta + i][xstarta + j] != b[ystartb + i][xstartb + j]) return false;
		}
	}
	return true;
}


/*Returns pattern reflected about the x axis*/
template<typename T, unsigned N, unsigned M>
matrix<T, N, M> reflect(matrix<T, N, M>& pattern)
{
	matrix<T, N, M> reflected;
	for (unsigned i = 0; i < N; i++)
		for (unsigned j = 0; j < M; j++) {
			reflected[i][M - 1 - j] = pattern[i][j];
		}
	return reflected;
}

/*Returns the matrix rotated 90 degrees clockwise*/
template<typename T, unsigned N, unsigned M>
matrix<T, M, N> rotate90(matrix<T, N, M>& pattern)
{
	matrix<T, M, N> rotated;
	for (unsigned i = 0; i < N; i++)
		for (unsigned j = 0; j < M; j++) {
			rotated[j][N - 1 - i] = pattern[i][j];
		}
	return rotated;
}
