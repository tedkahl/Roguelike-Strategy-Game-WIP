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
void DrawComponent::setTextureRect(const sf::IntRect& rect) {
	if (rect.left == -1) {
		sprite.setTextureRect(default_rect);
		std::cout << "animation done " << to_str(default_rect);
	}
	else sprite.setTextureRect(rect);
	//std::cout << to_str(sprite.getTextureRect());
}

void DrawComponent::set(const sf::Texture& texture, const sf::Vector2f& offset, unsigned order, const sf::IntRect& rect, int batch)
{
	health = (order == 50) ? 1.f : -1.f;
	if (outer.getFillColor() != sf::Color::Black) {
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

	order_ = order;
	setMoveDirection(1, sortable());
	batch_ = batch;
}

bool DrawComponent::update(const sf::Time& current, const EntityUpdate& update, DCSortable& ref) {
	//cout << "updating dc" << endl;
	bool dirty = false;
	if (update.move_dir_ == 0 && move_direction == 1) {
		dirty = true;
		//cout << "updating move dir" << endl;
		setMoveDirection(0, ref);
	}
	if (update.animation_)
		setAnimation(current, update.animation_.value());

	if (update.coords_) {
		if (static_cast<signed char>(x) != update.coords_.value().x || static_cast<signed char>(zval_ - x) != update.coords_.value().y)
		{
			updateCoords(update.coords_.value(), ref);
			dirty = true;
		}
		//cout << "updating coords" << endl << to_string(update.coords_.value()) << endl;
	}
	if (update.sprite_position_) {
		//cout << "setting sprite pos " << to_string(update.sprite_position_.value()) << endl;
		setSpritePos(update.sprite_position_.value());
	}

	if (update.finished) {
		std::cout << "Action finished" << std::endl;
		if (move_direction != 1) {
			setMoveDirection(update.move_dir_, ref);
			dirty = true;
		}
	}
	assert(!lt_associated(ref.sortVal(), sortVal()));
	assert(!lt_associated(sortVal(), ref.sortVal()));
	return dirty;
}

void DrawComponent::updateEntityPos(sf::Vector2i newpos, matrix<float>& heightmap) {
	updateCoords(newpos, sortable());
	float sq_h = heightmap.at(newpos);

	sf::Vector2f pos = squarePosFromCoords(newpos, heightmap.width());
	setSpritePos(sf::Vector2f(pos.x, pos.y + sq_h));
}

void DrawComponent::setSquarePos(sf::Vector2i newpos, matrix<float>& heightmap) {
	updateCoords(newpos, sortable());
	setSpritePos(squarePosFromCoords(newpos, heightmap.width()));
}

sf::RectangleShape DrawComponent::inner({ 40.f, 10.f });
sf::RectangleShape DrawComponent::outer({ 40.f, 10.f });