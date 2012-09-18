#include <iostream>
#include "Calc.h"

int Calc::Random(int beginNum, int endNum)
{
	return ((rand()%(endNum - beginNum + 1)) + beginNum);
}