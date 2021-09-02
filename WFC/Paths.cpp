#include "Paths.h"

std::optional<std::vector<sf::Vector2i>> pathsGrid::getPath(sf::Vector2i& dest) {
	if (grid->at(dest).search == search) {
		std::vector<sf::Vector2i> ret(1, dest);
		map_node& node = grid->at(dest);
		while (node.prev.x != -1) {
			ret.push_back(node.prev);
			node = grid->at(node.prev);
		}
		return ret;
	}
	return std::nullopt;
}