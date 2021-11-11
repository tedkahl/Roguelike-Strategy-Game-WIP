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
#include "Data.h"
#include "DCSortable.h"
#include "Debug.h"
class Entity;
struct Board;
class Square;

using std::cout;
using std::endl;
class DrawComponent : public DCSortable {
private:
	static sf::RectangleShape inner;
	static sf::RectangleShape outer;
	sf::Vector2f sprite_offset;
	sf::Sprite sprite;
	sf::IntRect default_rect;
	std::variant<Entity*, Square*> owner_;
	Animation animation;
	float health = .8f;
public:
	DrawComponent() = default;

	void set(const sf::Texture& texture, const sf::Vector2f& offset, unsigned obj_height, const sf::IntRect& rect, int batch = -1);
	inline sf::Sprite const& getSprite() const { return sprite; }
	inline void setColor(sf::Color&& c) { sprite.setColor(c); }

	inline void setSpritePos(const sf::Vector2f& position) {
		//cout << "setting sprite pos " << position.x << " " << position.y << endl;
		sprite.setPosition(position + sprite_offset);
	}
	inline void setTextureRect(const sf::IntRect& rect) {
		if (rect.left == -1) {
			sprite.setTextureRect(default_rect);
			std::cout << "animation done " << to_str(default_rect);
		}
		else sprite.setTextureRect(rect);
		//std::cout << to_str(sprite.getTextureRect());
	}

	std::variant<Entity*, Square*> getOwner() const;
	void setOwner(Square* owner);
	void setOwner(Entity* owner);

	inline sf::Vector2f getOffset() { return sprite_offset; }

	void setAnimation(sf::Time start, const Animation& anim);
	void draw(sf::RenderTarget* target);
	bool updateAnimation(sf::Time current);

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