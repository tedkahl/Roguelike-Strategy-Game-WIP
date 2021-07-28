#pragma once
#include <type_traits>
template<typename T>
class matrix {
private:
	std::vector<T> data;
	size_t height_;
	size_t width_;
public:
	matrix() = default;
	matrix(size_t w, size_t h) :data(h* w), height_(h), width_(w) {}
	matrix(size_t w, size_t h, const T val) :data(h* w, val), height_(h), width_(w) { }
	matrix(size_t w, size_t h, std::initializer_list<T> i) :data(i), height_(h), width_(w) {}
	void set(size_t h, size_t w, const T& val);
	inline T& at(size_t x, size_t y) { return data[y * width_ + x]; }
	inline T const& at(size_t x, size_t y) const { return data[y * width_ + x]; }
	inline T& at(std::pair<size_t, size_t> coords) { return data[coords.second * width_ + coords.first]; }
	inline T const& at(std::pair<size_t, size_t> coords) const { return data[coords.second * width_ + coords.first]; }
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
void matrix<T>::set(size_t h, size_t w, const T& val) {
	data.assign(h * w, val);
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