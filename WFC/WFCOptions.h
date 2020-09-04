#ifndef WFCOPTIONS
#define WFCOPTIONS
struct WFCOptions
{
	unsigned n;
	size_t oheight;
	size_t owidth;
	bool rotate;
	bool reflect;

	WFCOptions(const size_t oheight_, const size_t owidth_, const unsigned n_, const bool rotate_, const bool reflect_);
};

WFCOptions::WFCOptions(size_t oheight_, size_t owidth_, unsigned n_, bool rotate_, bool reflect_)
	:n(n_),oheight(oheight_),owidth(owidth_),rotate(rotate_),reflect(reflect_){}
#endif