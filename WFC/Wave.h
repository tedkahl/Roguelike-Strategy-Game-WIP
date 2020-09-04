#pragma once
#include <vector>
#include "WFCOptions.h"
template<class T>
class Wave
{
private:
	std::vector<std::vector<std::vector<std::vector<std::vector<int>>>>> wave;
public:
	Wave(const WFCOptions &options, std::vector<std::vector<std::vector<int>>> &patterns, std::vector<int> &pattern_weights);
	void initWave();

};
/*
template<class T>
void Wave<T>::initWave() {
	for (size_t i = 0;i<options.height;i+=options.n)
		for (size_t j = 0; j < options.width; j += options.n)
		{

		}
			
}*/