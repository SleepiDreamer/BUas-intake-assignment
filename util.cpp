#include <iostream>
#include "template.h"
#include "surface.h"
#include <cmath>

namespace Tmpl8
{
	int nthDigit(const int number, const int n)
	{
		return static_cast<int>(number / pow(10, n)) % 10;
	}

	std::tuple<int, int> IndexToXY(const int index, const int width)
	{
		return { index % width, floor(index / width) };
	}

	float distanceBetween(const float x1, const float y1, const float x2, const float y2)
	{
		return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
	}

	float RotationBetweenPoints(const float x1, const float y1, const float x2, const float y2) // using code from https://www.geeksforgeeks.org/program-find-slope-line/
	{
		if (x2 - x1 != 0) {
			bool toLeft = x1 > x2;
			if (toLeft) return atan((y2 - y1) / (x2 - x1)) + PI;
			//std::cout << atan((y2 - y1) / (x2 - x1)) << std::endl;
			return atan((y2 - y1) / (x2 - x1));
		}
		return 1.5 * PI;
	}

	float calcDir(const vec2& _vel)
	{
		return atan2(_vel.y, _vel.x);
	}

	float calcSpeed(const vec2& _vel)
	{
		return sqrt(_vel.x * _vel.x + _vel.y * _vel.y);
	}
}