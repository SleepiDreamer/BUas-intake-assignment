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
}