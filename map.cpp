#include <iostream>
#include <tuple>
#include <vector>

#include "surface.h"
#include "template.h"
#include "map.h"
#include "util.h"

namespace Tmpl8
{
	
	std::vector<std::vector<int>> map
	{
		{00, 00, 00, 00, 00, 00, 00, 00, 00},
		{00, 00, 11, 11, 11, 11, 11, 11, 00},
		{00, 11, 11, 11, 11, 11, 11, 11, 00},
		{00, 11, 11, 11, 11, 11, 11, 11, 00},
		{00, 11, 11, 11, 11, 11, 00, 00, 00},
		{00, 00, 00, 00, 00, 00, 00, 00, 00}
	};



	std::tuple<int, int, int, bool> Game::TileInfo(const int tx, const int ty)
	{
		int tx_r = floor(tx / mapWidth);
		int ty_r = floor(ty / mapHeight);
		return { tx_r, ty_r, nthDigit(map[ty][tx], 0), nthDigit(map[ty][tx], 1) };
	}

	void Game::RenderMap()
	{
		for (int i = 0; i < map[0].size(); i++)
		{
			for (int j = 0; j < map.size(); j++)
			{
				
			}
		}
	}
}
