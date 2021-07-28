#pragma once
#include <array>
#include <string>
#include <SFML/Graphics.hpp>
#include "ResourceManager.h"


class DrawComponent {
protected:
	std::shared_ptr<ResourceManager<sf::Texture>> tm_;
	sf::Vector2f sprite_offset;
	sf::Sprite sprite;
public:
	inline sf::Vector2f getOffset() { return sprite_offset; }
	void draw(sf::RenderTarget* target);
	void setSpritePos(sf::Vector2f pos) { sprite.setPosition(pos + sprite_offset); }
	inline sf::Sprite const& getSprite() const { return sprite; }
	inline void setSprite(const sf::Sprite& s) { sprite = s; }
	void updateSpritePos();
	DrawComponent() = default;
	void set(std::string path, sf::Vector2f& offset, ResourceManager<sf::Texture>& tm);
	DrawComponent(std::string path, sf::Vector2f& offset, ResourceManager<sf::Texture>& tm);
};

void DrawComponent::draw(sf::RenderTarget* target) {
	target->draw(sprite);
}

void DrawComponent::set(std::string path, sf::Vector2f& offset, ResourceManager<sf::Texture>& tm)
{
	tm_ = std::make_shared<sf::Texture>(&tm);
	sprite_offset = offset;
	sprite.setTexture(tm_->get(path));
}

DrawComponent::DrawComponent(std::string path, sf::Vector2f& offset, ResourceManager<sf::Texture>& tm) :tm_(&tm), sprite_offset(offset)
{
	sprite.setTexture(tm_->get(path));
}

void DrawComponent::updateSpritePos() {
	sprite.setPosition(parent->getSprite().getPosition() + sprite_offset);
	std::cout << (parent->getSprite()).getPosition().x << " " << (parent->getSprite()).getPosition().y << " " << sprite.getPosition().x << " " << sprite.getPosition().y << std::endl;
}