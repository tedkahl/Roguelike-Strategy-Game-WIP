#pragma once
#include "Square.h"
//#include "DataManager.h"
#include "matrix.h"
class Square;
struct BoardState {
	matrix<Square> board;
};