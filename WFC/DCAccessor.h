#pragma once
#include "DrawComponent.h"
#include "SortedDManager.h"
class DCAccessor {
private:
	unsigned zval;
	SortedDManager<DrawComponent, DCAccessor>& manager;
public:
	DCAccessor(DrawComponent& dc, SortedDManager<DrawComponent, DCAccessor>& m);
	friend bool operator<(const DCAccessor& l, const DrawComponent& r) { return l.zval < r.zval(); }
	friend bool operator==(const DCAccessor& l, const DrawComponent& r) { return l.zval == r.zval(); }
	DrawComponent* operator->();
	void setZval(unsigned new_zv);
};