#pragma once
#include <type_traits>
#include <SFML/Graphics.hpp>
#include <assert.h>
template<typename T>
class matrix {
private:
	std::vector<T> data;
	size_t height_;
	size_t width_;
public:
	matrix() :data(), height_(0), width_(0) {}
	matrix(size_t w, size_t h) :data(h* w), height_(h), width_(w) {}
	matrix(size_t w, size_t h, const T val) :data(h* w, val), height_(h), width_(w) { }
	matrix(size_t w, size_t h, std::initializer_list<T> i) :data(i), height_(h), width_(w) {}

	void set(size_t w, size_t h, const T& val = T());
	void resize(size_t w, size_t h);
	inline std::vector<T>& vec() { return data; }

	inline T& at(size_t x, size_t y) { return data[y * width_ + x]; }
	inline T const& at(size_t x, size_t y) const { assert(x < width_&& y < height_); return data[y * width_ + x]; }
	inline T& at(std::pair<size_t, size_t> coords) { assert(coords.first < width_&& coords.second < height_); return data[coords.second * width_ + coords.first]; }
	inline T const& at(std::pair<size_t, size_t> coords) const { assert(coords.first < width_&& coords.second < height_); return data[coords.second * width_ + coords.first]; }
	inline T& at(sf::Vector2i coords) { assert(coords.x < static_cast<int>(width_) && coords.y < static_cast<int>(height_)); return data[coords.y * width_ + coords.x]; }
	inline T const& at(sf::Vector2i coords) const { assert(coords.x < static_cast<int>(width_) && coords.y < static_cast<int>(height_)); return data[coords.y * width_ + coords.x]; }

	inline bool operator==(const matrix<T>& rhs) const { return (data == rhs.data && height_ == rhs.height_ && width_ == rhs.width_); }
	inline size_t height() const { return height_; }
	inline size_t width() const { return width_; }

	typedef std::vector<T>::iterator iterator;
	typedef std::vector<T>::const_iterator const_iterator;
	const_iterator begin() const { return data.begin(); }
	const_iterator end() const { return data.end(); }
	iterator begin() { return data.begin(); }
	iterator end() { return data.end(); }
};

template<typename T>
void matrix<T>::set(size_t w, size_t h, const T& val) {
	data.assign(h * w, val);
	height_ = h;
	width_ = w;
}

template<typename T>
void matrix<T>::resize(size_t w, size_t h) {
	data.resize(h * w);
	height_ = h;
	width_ = w;
}

namespace std {
	template <typename T>
	class hash<matrix<T>> {
	public:
		size_t operator()(const matrix<T>& m) const noexcept {
			size_t seed = sizeof m;
			for (auto i : m) {
				seed ^= std::hash<T>{}(i)+0x9e3779b9 + (seed << 6) + (seed >> 2); //based on Boost's hash_combine

			}
			return seed;
		}
	};
}