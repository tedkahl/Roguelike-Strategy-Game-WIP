#include "Paths.h"
template<typename T>
static void reverse(std::vector<T>& v) {
	for (unsigned i = 0;i < v.size() / 2;i++) {
		std::swap(v[i], v[v.size() - 1 - i]);
	}
}
std::optional<std::vector<sf::Vector2i>> pathsGrid::getPath(sf::Vector2i& dest) {
	auto loc = dest - offset;
	std::cout << "Moving to " << dest.x << ", " << dest.y << std::endl;
	if (loc.x < 0 || loc.x >= grid.width() || loc.y < 0 || loc.y >= grid.height()) return std::nullopt;
	if (is_movable(loc)) {
		std::vector<sf::Vector2i> ret(1, dest);
		map_node& node = grid.at(loc);
		while (node.prev.x != -1) {
			ret.push_back(node.prev);
			node = grid.at(node.prev - offset);
		}
		reverse(ret);
		return ret;
	}
	return std::nullopt;
}