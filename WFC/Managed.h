#pragma once
class Managed {
protected:
	unsigned index_;
public:
	Managed() = default;
	Managed(unsigned index) :index_(index) {}
	inline unsigned index() const { return index_; }
	inline void setIndex(unsigned index) { index_ = index; }
};