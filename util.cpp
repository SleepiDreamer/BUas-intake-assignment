#include <iostream>
#include "template.h"
#include "surface.h"

namespace Tmpl8
{
	int nthDigit(const int number, const int n)
	{
		return (int)(number / pow(10, n)) % 10;
	}

	std::tuple<int, int> IndexToXY(const int index, const int width)
	{
		return { index % width, floor(index / width) };
	}
}