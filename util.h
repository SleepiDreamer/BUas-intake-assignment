#pragma once
#include <iostream>
#include "template.h"
#include "surface.h"

namespace Tmpl8
{
	int nthDigit(const int number, const int n);
	std::tuple<int, int> IndexToXY(const int index, const int width);
}