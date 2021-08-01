#pragma once
#include <array>
#include <string>
#include <SFML/Graphics.hpp>
#include "ResourceManager.h"
#include "BoardState.h"
#include "Entity.h"
class BoardEntity;
struct BoardState;
class Square;

class DrawComponent {
private:
	std::shared_ptr<ResourceManager<sf::Texture>> tm_;
	sf::Vector2f sprite_offset;
	sf::Sprite sprite;
	unsigned index_;
	BoardEntity* owner_;
	unsigned zval_;
	unsigned priority_;
public:
	DrawComponent() = default;
	DrawComponent(std::string path, sf::Vector2f& offset, std::shared_ptr<ResourceManager<sf::Texture>> tm);
	inline unsigned index() const { return index_; }
	inline void setIndex(unsigned index) { index_ = index; }
	void set(std::string path, sf::Vector2f& offset, std::shared_ptr<ResourceManager<sf::Texture>>, unsigned priority, unsigned index);
	inline sf::Sprite const& getSprite() const { return sprite; }
	inline void setSpritePos(const sf::Vector2f& position) { sprite.setPosition(position + sprite_offset); }
	BoardEntity* getOwner();
	void setOwner(BoardEntity* owner);
	inline sf::Vector2f getOffset() { return sprite_offset; }
	void draw(sf::RenderTarget* target);
	void updateEntityPos(std::pair<unsigned, unsigned> newpos, BoardState& state);
	void setSquarePos(std::pair<unsigned, unsigned> newpos, BoardState& state);
	friend bool operator<(const DrawComponent& l, const DrawComponent& r) { return l.zval_ < r.zval_; }
};
