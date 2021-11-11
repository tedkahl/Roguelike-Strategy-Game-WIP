#include "DrawComponent.h"
#include "Board.h"

void DrawComponent::draw(sf::RenderTarget* target) {
	target->draw(sprite);
	if (health >= 0.f) {
		inner.setPosition(sprite.getPosition() + sprite_offset + sf::Vector2f{ 16.f, 30.f });
		outer.setPosition(sprite.getPosition() + sprite_offset + sf::Vector2f{ 16.f, 30.f });
		inner.setSize({ health * 40.f, 10.f });
		target->draw(outer);
		target->draw(inner);
	}
}
void DrawComponent::setAnimation(sf::Time start, const Animation& anim) {
	//cout << "setting animation" << endl;
	animation = anim;
	animation.start(start);
}
bool DrawComponent::updateAnimation(sf::Time current) {
	if (animation.active()) {
		setTextureRect(animation.getRect(current, sprite.getTexture()->getSize().x));
		return true;
	}
	return false;
}

void DrawComponent::set(const sf::Texture& texture, const sf::Vector2f& offset, unsigned obj_height, const sf::IntRect& rect, int batch)
{
	if (obj_height != 2) health = -1.f;
	else {
		outer.setFillColor(sf::Color::Black);
		outer.setOutlineColor(sf::Color::Black);
		outer.setOutlineThickness(2.f);
		inner.setFillColor(sf::Color::Red);
	}

	sprite_offset = offset;
	sprite = sf::Sprite(texture);
	if (rect != sf::IntRect()) {
		sprite.setTextureRect(rect);
	}
	default_rect = sprite.getTextureRect();

	obj_height_ = obj_height;
	setMoveDirection(1, sortable());
	batch_ = batch;
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

sf::RectangleShape DrawComponent::inner({ 40.f, 10.f });
sf::RectangleShape DrawComponent::outer({ 40.f, 10.f });