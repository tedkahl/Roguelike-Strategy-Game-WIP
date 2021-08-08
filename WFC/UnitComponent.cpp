#include "UnitComponent.h"

void UnitComponent::updatePointers(UnitComponent& removed) {
	owner_->setUC(this);
}

BoardEntity* UnitComponent::getOwner() { return owner_; }
void UnitComponent::setOwner(BoardEntity* owner) {
	owner_ = owner;
}