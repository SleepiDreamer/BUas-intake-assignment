#pragma once
#include <iostream>
#include "template.h"
#include "surface.h"

namespace Tmpl8
{
	/**
	 * \brief returns the nth digit of an integer
	 * \param number  the number you want to use
	 * \param n  which digit to return
	 * \return digit "n" of "number"
	 */
	int nthDigit(const int number, const int n);

	/**
	 * \brief returns an XY coordinate based on an index and width. Mostly useful for spritesheets
	 * \param index  the index entry of the spritesheet you want (left to right, top to bottom)
	 * \param width  the width of the spritesheet (in entries)
	 * \return the sprite's entryX and entryY coordinate in the spritesheet as a tuple of two integers
	 */
	std::tuple<int, int> IndexToXY(const int index, const int width);

	/**
	 * \return the distance between p1 and p2 (float)
	 */
	float distanceBetween(const float x1, const float y1, const float x2, const float y2);
	inline float distanceBetween(const vec2& pos1, const vec2& pos2)
	{
		return distanceBetween(pos1.x, pos1.y, pos2.x, pos2.y);
	}

	/**
	 * \brief returns the direction (in radians) someone at p1 should point towards if it were to look at p2
	 * \param x1  your x
	 * \param y1  your y
	 * \param x2  target x
	 * \param y2  target y
	 * \return 
	 */
	float RotationBetweenPoints(const float x1, const float y1, const float x2, const float y2);

	const vec2& calcDir(const vec2& _vel);

	float calcSpeed(const vec2& _vel);

	inline float RotationBetweenPoints(const vec2& pos1, const vec2& pos2)
	{
		return RotationBetweenPoints(pos1.x, pos1.y, pos2.x, pos2.y);
	}

	vec2 reflectVector(vec2& _vel, const vec2& _normal);

	float vec2ToAngle(const vec2& _vel);

	int randint(const int _min, const int _max, float _time);

	float randfloat(const float _min, const float _max);

	bool insideScreen(vec2 _pos);
}