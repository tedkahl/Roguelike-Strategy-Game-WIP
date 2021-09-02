#pragma once
#include <array>
#include <string>
#include <SFML/Graphics.hpp>
#include <variant>
#include "Entity.h"
#include "ResourceManager.h"
#include "BoardState.h"
#include "Data.h"
class BoardEntity;
struct BoardState;
class Square;

class DrawComponent {
private:
	std::shared_ptr<ResourceManager<sf::Texture>> tm_;
	sf::Vector2f sprite_offset;
	sf::Sprite sprite;

	unsigned index_;
	std::variant<BoardEntity*, Square*> owner_;
	unsigned zval_;
	unsigned priority_;
public:
	DrawComponent() = default;
	DrawComponent(std::string path, sf::Vector2f& offset, std::shared_ptr<ResourceManager<sf::Texture>> tm);
	inline unsigned index() const { return index_; }
	inline void setIndex(unsigned index) { index_ = index; }
	void set(std::string path, sf::Vector2f& offset, std::shared_ptr<ResourceManager<sf::Texture>>, unsigned priority, sf::IntRect& rect, unsigned index);
	inline sf::Sprite const& getSprite() const { return sprite; }
	inline void setSpritePos(const sf::Vector2f& position) { sprite.setPosition(position + sprite_offset); }
	std::variant<BoardEntity*, Square*> getOwner();
	void setOwner(Square* owner);
	void setOwner(BoardEntity* owner);
	inline sf::Vector2f getOffset() { return sprite_offset; }
	//void update(sf::Vector2f& pos);
	void draw(sf::RenderTarget* target) const;
	void updateEntityPos(sf::Vector2i newpos, BoardState& state);
	void setSquarePos(sf::Vector2i newpos, BoardState& state);
	void updatePointers(DrawComponent& removed);
	friend bool operator<(const DrawComponent& l, const DrawComponent& r) { return l.zval_ < r.zval_; }
};