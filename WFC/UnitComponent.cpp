#include "UnitComponent.h"

void UnitComponent::updatePointers(UnitComponent& removed) {
	owner_->setUC(this);
}

Entity* UnitComponent::getOwner() const { return owner_; }
void UnitComponent::setOwner(Entity* owner) {
	owner_ = owner;
}

sf::Vector2i UnitComponent::getPos() const { return owner_->getPos(); }
//AIComponent* UnitComponent::AI() const { return AI_.get(); }

bool UnitComponent::damage(int attack) {
	current_hp -= attack;
	getOwner()->dc()->setHealth(static_cast<float>(current_hp) / stats_.max_hp);
	return current_hp < 0;
}