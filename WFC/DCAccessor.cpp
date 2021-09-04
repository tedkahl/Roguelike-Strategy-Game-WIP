#include "DCAccessor.h"

DCAccessor::DCAccessor(DrawComponent& dc, SortedDManager<DrawComponent, DCAccessor>& m) : zval(dc.zval()), manager(m) {}

DrawComponent* DCAccessor::operator->() {
	return manager.get(*this);
}

void DCAccessor::setZval(unsigned new_zv)
{
	DrawComponent* tochange = manager.get(*this);
	tochange->setZval(new_zv);
	manager.fixChangedVal(tochange);
	zval = new_zv;
}