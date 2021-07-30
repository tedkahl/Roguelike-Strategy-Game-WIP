#include "DrawComponent.h"

void DrawComponent::draw(sf::RenderTarget* target) {
	target->draw(sprite);
}

void DrawComponent::set(std::string path, sf::Vector2f& offset, std::shared_ptr<ResourceManager<sf::Texture>> tm, unsigned index)
{
	tm_ = tm;
	sprite_offset = offset;
	sprite.setTexture(tm_->get(path));
	index_ = index;
}

DrawComponent::DrawComponent(std::string path, sf::Vector2f& offset, std::shared_ptr<ResourceManager<sf::Texture>> tm) :tm_(tm), sprite_offset(offset)
{
	sprite.setTexture(tm_->get(path));
}


BoardEntity* DrawComponent::getOwner() { return owner_; }
void DrawComponent::setOwner(BoardEntity* owner) { owner_ = owner; }

void DrawComponent::updatePos(std::pair<unsigned, unsigned> newpos, BoardState& state) {
	zval = newpos.first * state.board.width() + newpos.second + priority;
	sprite.setPosition(state.board.at(newpos).dc()->getSprite().getPosition() + sprite_offset);
}