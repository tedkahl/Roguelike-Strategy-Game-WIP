#include "DrawComponent.h"

void DrawComponent::draw(sf::RenderTarget* target) const {
	target->draw(sprite);
}

void DrawComponent::set(std::string path, sf::Vector2f& offset, std::shared_ptr<ResourceManager<sf::Texture>> tm, unsigned priority, sf::IntRect& rect, unsigned index)
{
	tm_ = tm;
	sprite_offset = offset;
	sprite = std::move(sf::Sprite(tm_->get(path)));
	if (rect != sf::IntRect()) sprite.setTextureRect(rect);
	priority_ = priority;
	index_ = index;
}

DrawComponent::DrawComponent(std::string path, sf::Vector2f& offset, std::shared_ptr<ResourceManager<sf::Texture>> tm) :tm_(tm), sprite_offset(offset)
{
	sprite.setTexture(tm_->get(path));
}

std::variant<BoardEntity*, Square*> DrawComponent::getOwner() { return owner_; }
void DrawComponent::setOwner(Square* owner) { owner_ = owner; }
void DrawComponent::setOwner(BoardEntity* owner) { owner_ = owner; }

void DrawComponent::updateEntityPos(std::pair<unsigned, unsigned> newpos, BoardState& state) {
	zval_ = (newpos.second * state.board.width() + newpos.first) * 10 + priority_;
	setSpritePos(state.board.at(newpos).dc()->getSprite().getPosition());
}

void DrawComponent::setSquarePos(std::pair<unsigned, unsigned> newpos, BoardState& state) {
	zval_ = (newpos.second * state.board.width() + newpos.first) * 10 + priority_;
	setSpritePos(sf::Vector2f((state.board.width() - 1 + newpos.first - newpos.second) * sq::square_w / 2, (newpos.first + newpos.second) * sq::square_h / 2));
}

void DrawComponent::updatePointers(DrawComponent& removed)
{
	std::visit([&](auto ptr) {ptr->setDC(this);}, owner_);
}