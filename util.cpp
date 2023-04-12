#include <iostream>
#include "template.h"
#include "surface.h"
#include <cmath>
#include <time.h>
#include <random>

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


	vec2 reflectVector(vec2& _vel, const vec2& _normal)
	{
		vec3 vel = { _vel.x, _vel.y, 0 }; // convert to 3d vector because dot product is only implemented for 3d vectors
		vec3 normal = { _normal.x, _normal.y, 0 };
		vec2 result = 		
		{ 
			_vel.x - 2 * normal.x * (dot(normal, vel) / dot(normal, normal)), 
			_vel.y - 2 * normal.y * (dot(normal, vel) / dot(normal, normal))
		};
		return result;
	}

	float vec2ToAngle(const vec2& _vel)
	{
		return atan2(_vel.y, _vel.x);
	}

	int randint(const int _min, const int _max, float _time)
	{
		srand(static_cast<int>(_time));
		return rand() % (_max - _min + 1) + _min;
	}

	float randfloat(const float _min, const float _max)
	{
		return _min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (_max - _min)));
	}

	bool insideScreen(vec2 _pos)
	{
		return _pos.x > 0 && _pos.x < ScreenWidth && _pos.y > 0 && _pos.y < ScreenHeight;
	}
}