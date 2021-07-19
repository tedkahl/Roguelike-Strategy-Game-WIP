#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <algorithm>

template<typename T>
concept SFResource = requires(T t) {
	t.loadFromFile("");
};

class Drawable {
protected:
	std::string name;
	float x;
	float y;
	float width;
	float height;
	Drawable(std::string n);
	Drawable(std::string n, float sx, float sy, float w = 0, float h = 0);
public:
	void setLoc(float sx, float sy, float w = 0, float h = 0);
	virtual void draw(sf::RenderWindow& window) = 0;
};
Drawable::Drawable(std::string n) : name(n) {}
Drawable::Drawable(std::string n, float sx, float sy, float w, float h) : name(n), x(sx), y(sy), width(w), height(h) {}

void Drawable::setLoc(float sx, float sy, float w, float h) {
	x = sx;
	y = sy;
	if (w) width = w;
	if (h) height = h;
}

template<typename T>
requires SFResource<T>
class ResourceManager {
private:
	std::map<std::string, std::unique_ptr<T>> resources;
public:
	ResourceManager();
	ResourceManager(const ResourceManager<T>& other);
	T& get(std::string name);
};

template<typename T>
requires SFResource<T>
ResourceManager<T>::ResourceManager() {}


template<typename T>
requires SFResource<T>
ResourceManager<T>::ResourceManager(const ResourceManager<T>& other) :resources(other.resources) {}

template<typename T>
requires SFResource<T>
T& ResourceManager<T>::get(std::string name) {
	auto search = resources.find(name);
	if (search == resources.end()) {
		std::unique_ptr<T> resource(new T());
		if (!resource->loadFromFile(name)) std::cerr << "error loading " << name << std::endl;

		auto inserted = resources.emplace(std::make_pair(name, std::move(resource)));
		assert(inserted.second);
		return *inserted.first->second;
	}
	return *search->second;
}
//returns status code 0: fully drawn, 1: partially drawn but cut off, 2: not drawn
int drawWithinBounds(sf::Sprite& s, unsigned x, unsigned y, unsigned startX, unsigned startY, unsigned endX, unsigned endY) {
	unsigned posX, posY;
	sf::IntRect Trect;

	auto [w, h] = s.getTexture()->getSize();
	if (x + w < startX || x > endX || y + h < startY || y > endY) {
		return 2; //not drawn
	}

	posX = std::max(x, startX);
	posY = std::max(y, startY);
	Trect.left = posX - x;
	Trect.top = posY - y;
	Trect.width = std::min(w - Trect.left, endX - posX);
	Trect.height = std::min(y - Trect.top, endY - posY);

	s.setPosition(posX, posY);
	s.setTextureRect(Trect);

}