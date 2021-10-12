#include "DrawComponent.h"
void DrawComponent::draw(sf::RenderTarget* target) const {
	target->draw(sprite);
	if (health >= 0) {
		sf::RectangleShape outer({ 40.f, 10.f });
		outer.setOrigin(sprite.getPosition() + sprite_offset + sf::Vector2f{ 0.f, 20.f });
		outer.setFillColor(sf::Color::Black);
		outer.setOutlineColor(sf::Color::Black);
		outer.setOutlineThickness(2.f);

		sf::RectangleShape inner({ health * 40.f, 10.f });
		inner.setOrigin(sprite.getPosition() + sprite_offset + sf::Vector2f{ 0.f, 20.f });
		inner.setFillColor(sf::Color::Red);
		target->draw(outer);
		target->draw(inner);
	}
}
void DrawComponent::setAnimation(sf::Time start, std::vector<AnimationSeg>&& segs, bool loop, float speed) {
	//cout << "setting animation" << endl;
	animation.set(start, std::forward< std::vector<AnimationSeg>>(segs), loop, speed);
}
bool DrawComponent::updateAnimation(sf::Time current) {
	if (animation.active()) {
		setTextureRect(animation.getRect(current, sprite.getTexture()->getSize().x));
		return true;
	}
	return false;
}

void DrawComponent::set(std::string path, sf::Vector2f& offset, std::shared_ptr<ResourceManager<sf::Texture>> tm, unsigned obj_height, const sf::IntRect& rect, int batch)
{
	if (obj_height == 0) health = -1.f;
	tm_ = tm;
	sprite_offset = offset;
	sprite = std::move(sf::Sprite(tm_->get(path)));
	if (rect != sf::IntRect()) {
		sprite.setTextureRect(rect);
	}
	default_rect = sprite.getTextureRect();

	obj_height_ = obj_height;
	setMoveDirection(1, sortable());
	batch_ = batch;
}

DrawComponent::DrawComponent(std::string path, sf::Vector2f& offset, std::shared_ptr<ResourceManager<sf::Texture>> tm) :tm_(tm), sprite_offset(offset)
{
	sprite.setTexture(tm_->get(path));
}

std::variant<Entity*, Square*> DrawComponent::getOwner() const { return owner_; }
void DrawComponent::setOwner(Square* owner) { owner_ = owner; }
void DrawComponent::setOwner(Entity* owner) { owner_ = owner; }

void DrawComponent::updateEntityPos(sf::Vector2i newpos, matrix<float>& heightmap) {
	updateCoords(newpos, sortable());
	float sq_h = heightmap.at(newpos);
	square_height_ = static_cast<uint8_t>(-sq_h + 10);

	sf::Vector2f pos = squarePosFromCoords(newpos, heightmap.width());
	setSpritePos(sf::Vector2f(pos.x, pos.y + sq_h));
}

void DrawComponent::setSquarePos(sf::Vector2i newpos, matrix<float>& heightmap) {
	updateCoords(newpos, sortable());
	square_height_ = static_cast<uint8_t>(-(sprite_offset.y) + 10);
	setSpritePos(squarePosFromCoords(newpos, heightmap.width()));
}
