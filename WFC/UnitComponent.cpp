#include "UnitComponent.h"
#include "Entity.h"

void UnitComponent::updatePointers(UnitComponent& removed) {
	owner_->setUC(this);
}

Entity* UnitComponent::getOwner() { return owner_; }
void UnitComponent::setOwner(Entity* owner) {
	owner_ = owner;
}

sf::Vector2i UnitComponent::getPos() { return owner_->getPos(); }