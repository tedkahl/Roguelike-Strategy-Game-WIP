#include "DrawComponent.h"

void DrawComponent::draw(sf::RenderTarget* target) {
	target->draw(sprite);
}

void DrawComponent::set(std::string path, sf::Vector2f& offset, std::shared_ptr<ResourceManager<sf::Texture>> tm, unsigned priority, unsigned index)
{
	tm_ = tm;
	sprite_offset = offset;
	sprite.setTexture(tm_->get(path));
	priority_ = priority;
	index_ = index;
}

DrawComponent::DrawComponent(std::string path, sf::Vector2f& offset, std::shared_ptr<ResourceManager<sf::Texture>> tm) :tm_(tm), sprite_offset(offset)
{
	sprite.setTexture(tm_->get(path));
}


BoardEntity* DrawComponent::getOwner() { return owner_; }
void DrawComponent::setOwner(BoardEntity* owner) { owner_ = owner; }

void DrawComponent::updateEntityPos(std::pair<unsigned, unsigned> newpos, BoardState& state) {
	zval_ = (newpos.first * state.board.width() + newpos.second) * 10 + priority_;
	setSpritePos(state.board.at(newpos).dc()->getSprite().getPosition());
}

void DrawComponent::setSquarePos(std::pair<unsigned, unsigned> newpos, BoardState& state) {
	zval_ = (newpos.first * state.board.width() + newpos.second) * 10 + priority_;
	setSpritePos(sf::Vector2f((state.board.width() - 1 + newpos.first - newpos.second) * sq::square_w / 2, (newpos.first + newpos.second) * sq::square_h / 2));
}