#include <fstream>
#include "template.h"
#include <cmath>
#include <random>
#include <string>

#define HYPOT(x, y) sqrt((x) * (x) + (y) * (y)) // used for function 'distanceToRect'

namespace Tmpl8
{
	/**
	 * \brief returns the nth digit of an integer
	 * \param number  the number you want to use
	 * \param n  which digit to return
	 * \return digit "n" of "number"
	 */
	int nthDigit(const int number, const int n)
	{
		return static_cast<int>(number / pow(10, n)) % 10;
	}

	// returns the distance between two points
	float distanceBetween(const float x1, const float y1, const float x2, const float y2)
	{
		return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
	}

	// returns the direction (in radians) someone at p1 should point towards if it were to look at p2
	float RotationBetweenPoints(const float x1, const float y1, const float x2, const float y2) // using code from https://www.geeksforgeeks.org/program-find-slope-line/
	{
		if (x2 - x1 != 0.0f) {
			if (x1 > x2) return atan((y2 - y1) / (x2 - x1)) + PI;
			return atan((y2 - y1) / (x2 - x1));
		}
		return 1.5f * PI;
	}

	// returns the direction (in radians) someone at p1 should point towards if they were to look at p2, but in a different way than RotationBetweenPoints
	float calcDir(const vec2& _vel)
	{
		return atan2(_vel.y, _vel.x);
	}

	// returns the scalar speed of a given velocity vector
	float calcSpeed(const vec2& _vel)
	{
		return sqrt(_vel.x * _vel.x + _vel.y * _vel.y);
	}

	// calculates the reflection of a vector on a surface with a given normal
	vec2 reflectVector(const vec2& _vel, const vec2& _normal)
	{
		const vec3 vel = { _vel.x, _vel.y, 0 }; // convert to 3d vector because dot product is only implemented for 3d vectors
		const vec3 normal = { _normal.x, _normal.y, 0 };
		const vec2 result = 		
		{ 
			_vel.x - 2 * normal.x * (dot(normal, vel) / dot(normal, normal)), 
			_vel.y - 2 * normal.y * (dot(normal, vel) / dot(normal, normal))
		};
		return result;
	}

	// converts a vector to an angle in radians
	float vec2ToAngle(const vec2& _vel)
	{
		return atan2(_vel.y, _vel.x);
	}

	// returns a random integer between _min and _max
	int randint(const int _min, const int _max)
	{
		static std::random_device rd;
		static std::mt19937 gen(rd());
		std::uniform_int_distribution<int> dis(_min, _max);
		return dis(gen);
	}

	// returns a random float between _min and _max
	float randfloat(const float _min, const float _max)
	{
		static std::random_device rd;
		static std::mt19937 gen(rd());
		std::uniform_real_distribution<float> dis(_min, _max);
		return dis(gen);
	}

	// checks if the given point is inside the screen
	bool insideScreen(vec2 _pos)
	{
		return _pos.x > 0 && _pos.x < ScreenWidth && _pos.y > 0 && _pos.y < ScreenHeight;
	}

	// checks if the given point is inside the given rectangle
	bool insideRect(vec2 _point, vec2 _pos1, vec2 _pos2)
	{
		return _point.x > _pos1.x && _point.x < _pos2.x && _point.y > _pos1.y && _point.y < _pos2.y;
	}

	// rounds a float to a given number of decimals
	float roundNumber(float _number, int _dec)
	{
		float mult = 10;
		for (int i = 0; i < _dec; i++) mult *= 10;
		mult /= 10;
		return std::round(_number * mult) / mult;
	}

	// adapted from Armen Tsirunyan's answer on https://www.appsloveworld.com/cplus/100/81/getting-the-nth-line-of-a-text-file-in-c
	// returns the string at the nth line of a text file
	std::string readNthLine(const std::string& _filename, int _n)
	{
		std::ifstream in(_filename.c_str());
		std::string s;
		//for performance
		s.reserve(300);

		//skip _n lines
		for (int i = 0; i < _n; ++i)
			std::getline(in, s);

		std::getline(in, s);
		in.close();
		return s;
	}

	// writes a string to a text file
	void writeToFile(const std::string& _filename, int _score)
	{
		std::fstream file;
		file.open(_filename, std::ios::out);
		file << _score;
		file.close();
	}

	//https://www.shadertoy.com/view/MtBGWc
	// returns the smallest distance between a given point and any point on a rectangle
	float distanceToRect(double x, double y, double x_min, double y_min, double x_max, double y_max)
	{
		if (x < x_min) {
			if (y < y_min) return HYPOT(x_min - x, y_min - y);
			if (y <= y_max) return x_min - x;
			return HYPOT(x_min - x, y_max - y);
		}
		else if (x <= x_max) {
			if (y < y_min) return y_min - y;
			if (y <= y_max) return 0;
			return y - y_max;
		}
		else {
			if (y < y_min) return HYPOT(x_max - x, y_min - y);
			if (y <= y_max) return x - x_max;
			return HYPOT(x_max - x, y_max - y);
		}
	}

	// returns the smallest distance between a given point and any point on a rectangle
	float distanceToRect(vec2 _point, vec2 _pos1, vec2 _pos2)
	{
		return distanceToRect(static_cast<int>(_point.x), static_cast<int>(_point.y), static_cast<int>(_pos1.x), static_cast<int>(_pos1.y), static_cast<int>(_pos2.x), static_cast<int>(_pos2.y));
	}

	// combines red, green and blue values into a single integer color
	int rgbCombine(int r, int g, int b)
	{
		return (r << 16) + (g << 8) + b;
	}
}
