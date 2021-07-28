#pragma once
#include <array>
#include "Drawable.h"
class DCManager {
private:
	static const unsigned MAX = 200;
	std::array<DrawComponent, MAX> data;
public:
	unsigned active = 0;
	std::pair<DrawComponent*, unsigned> getNew();
	unsigned deactivate(unsigned index);
};

std::pair<DrawComponent*, unsigned> DCManager::getNew()
{
	return std::make_pair(&data[active], active);
}

unsigned DCManager::deactivate(unsigned index) {
	std::swap(data[index], data[active - 1]);
	data[index].updatePointer()
}