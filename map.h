#pragma once

#include <iostream>
#include "template.h"
#include "surface.h"
#include <vector>

namespace Tmpl8
{
	class Game
	{
	public:
		
		std::tuple<int, int, int, bool> TileInfo(const int tx, const int ty);
		void RenderMap();

		Game() :
			mapSprite(new Surface("assets/mapTiles.png"), 1)
		{
		}
	private:
		Sprite mapSprite;
		const int mapWidth = 18, mapHeight = 6, mapParameters = 2;
	};
}
