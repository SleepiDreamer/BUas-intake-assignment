#include <iostream>
#include <fstream>
#include "template.h"
#include <cmath>
#include <random>
#include <string>
#include <charconv>

namespace Tmpl8
{
	int nthDigit(const int number, const int n)
	{
		return static_cast<int>(number / pow(10, n)) % 10;
	}

	float distanceBetween(const float x1, const float y1, const float x2, const float y2)
	{
		return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
	}

	float RotationBetweenPoints(const float x1, const float y1, const float x2, const float y2) // using code from https://www.geeksforgeeks.org/program-find-slope-line/
	{
		if (x2 - x1 != 0.0f) {
			if (x1 > x2) return atan((y2 - y1) / (x2 - x1)) + PI;
			return atan((y2 - y1) / (x2 - x1));
		}
		return 1.5f * PI;
	}

	float calcDir(const vec2& _vel)
	{
		return atan2(_vel.y, _vel.x);
	}

	float calcSpeed(const vec2& _vel)
	{
		return sqrt(_vel.x * _vel.x + _vel.y * _vel.y);
	}


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

	float vec2ToAngle(const vec2& _vel)
	{
		return atan2(_vel.y, _vel.x);
	}

	int randint(const int _min, const int _max)
	{
		static std::random_device rd;
		static std::mt19937 gen(rd());
		std::uniform_int_distribution<int> dis(_min, _max);
		return dis(gen);
	}

	float randfloat(const float _min, const float _max)
	{
		static std::random_device rd;
		static std::mt19937 gen(rd());
		std::uniform_real_distribution<float> dis(_min, _max);
		return dis(gen);
	}

	bool insideScreen(vec2 _pos)
	{
		return _pos.x > 0 && _pos.x < ScreenWidth && _pos.y > 0 && _pos.y < ScreenHeight;
	}

	char* stringToCString(const std::string& str) {
		char* charPtr = new char[str.length() + 1];
		std::strcpy(charPtr, str.c_str()); // TODO: don't use heap
		return charPtr;
	}

	bool insideRect(vec2 _point, vec2 _pos1, vec2 _pos2)
	{
		return _point.x > _pos1.x && _point.x < _pos2.x && _point.y > _pos1.y && _point.y < _pos2.y;
	}

	float roundNumber(float _number, int _dec)
	{
		float mult = 10;
		for (int i = 0; i < _dec; i++) mult *= 10;
		mult /= 10;
		return std::round(_number * mult) / mult;
	}

	// adapted from Armen Tsirunyan's answer on https://www.appsloveworld.com/cplus/100/81/getting-the-nth-line-of-a-text-file-in-c
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

	void writeToFile(const std::string& _filename, int _score)
	{
		std::fstream file;
		file.open(_filename, std::ios::out);
		file << _score;
		file.close();
	}
}
