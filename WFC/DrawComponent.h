#pragma once
#include <array>
#include <string>
#include <SFML/Graphics.hpp>
#include <variant>
#include "matrix.h"
#include "SortedDManager.h"
#include "Entity.h"
#include "Animation.h"
#include "ResourceManager.h"
#include "Board.h"
#include "Data.h"
#include "DCSortable.h"
class Entity;
struct Board;
class Square;

class DrawComponent : public DCSortable {
private:
	std::shared_ptr<ResourceManager<sf::Texture>> tm_;
	sf::Vector2f sprite_offset;
	sf::Sprite sprite;
	sf::IntRect default_rect;
	Animation* anim;

	std::variant<Entity*, Square*> owner_;
public:
	DrawComponent() = default;
	DrawComponent(std::string path, sf::Vector2f& offset, std::shared_ptr<ResourceManager<sf::Texture>> tm);

	void set(std::string path, sf::Vector2f& offset, std::shared_ptr<ResourceManager<sf::Texture>>, unsigned obj_height, const sf::IntRect& rect, int batch = -1);
	inline sf::Sprite const& getSprite() const { return sprite; }
	inline void setSpritePos(const sf::Vector2f& position) { sprite.setPosition(position + sprite_offset); }
	inline void setTextureRect(const sf::IntRect& rect) {
		if (rect.left == -1)
			sprite.setTextureRect(default_rect);
		else sprite.setTextureRect(rect);
	}

	std::variant<Entity*, Square*> getOwner() const;
	void setOwner(Square* owner);
	void setOwner(Entity* owner);

	inline sf::Vector2f getOffset() { return sprite_offset; }

	void draw(sf::RenderTarget* target) const;

	void updateEntityPos(sf::Vector2i newpos, matrix<float>& heightmap);
	void setSquarePos(sf::Vector2i newpos, matrix<float>& heightmap);
	friend bool operator<(const DrawComponent& l, const DrawComponent& r) { return l.sortVal() < r.sortVal(); }
	friend bool operator<(const DrawComponent& l, const SortType& r) { return l.sortVal() < r; }
	friend bool operator==(const DrawComponent& l, const SortType& r) {
		return l.sortVal() == r;
	}
};

static sf::Vector2f squarePosFromCoords(sf::Vector2i pos, unsigned board_w) {
	return sf::Vector2f((board_w - 1 + pos.x - pos.y) * sq::square_w / 2, (pos.x + pos.y) * sq::square_h / 2);
}
