#include <iostream>
#include <tuple>
#include <vector>

#include "surface.h"
#include "template.h"
#include "map.h"
#include "util.h"

namespace Tmpl8
{
	
	std::vector<std::vector<int>> map // TODO change to vec2
	{
		{00, 00, 00, 00, 00, 00, 00, 00, 00},
		{00, 00, 11, 11, 11, 11, 11, 11, 00},
		{00, 11, 11, 11, 11, 11, 11, 11, 00},
		{00, 11, 11, 11, 11, 11, 11, 11, 00},
		{00, 11, 11, 11, 11, 11, 00, 00, 00},
		{00, 00, 00, 00, 00, 00, 00, 00, 00}
	};



	std::tuple<int, int, int, bool> Map::TileInfo(const int tx, const int ty) // TODO change to vec2
	{
		int tx_r = floor(tx / mapWidth);
		int ty_r = floor(ty / mapHeight);
		return { tx_r, ty_r, nthDigit(map[ty][tx], 0), nthDigit(map[ty][tx], 1) == 1 };
	}

	void Map::DrawTile(int tx, int ty, Surface* screen, int x, int y)
	{
		Pixel* src = mapSprite.GetBuffer() + 1 + tx * 33 + (1 + ty * 33) * 595;
		Pixel* dst = screen->GetBuffer() + x + y * ScreenWidth;
		for (int i = 0; i < 32; i++, src += 595, dst += ScreenWidth)
			for (int j = 0; j < 32; j++)
				dst[j] = src[j];
	}

	void Map::RenderMap(Surface* gameScreen, const int startX, const int startY)
	{
		for (int tileX = 0; tileX < map[0].size(); tileX++)
		{
			for (int tileY = 0; tileY < map.size(); tileY++)
			{
				int renderX = tileX * 32;
				int renderY = tileY * 32;
				auto tile = TileInfo(tileX, tileY);
				int mapX = std::get<2>(tile) * 32;
				int mapY = std::get<3>(tile) * 32;
				DrawTile(mapX, mapY, gameScreen, renderX, renderY);
			}
		}
	}
}
