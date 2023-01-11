#pragma once

#include <iostream>
#include "template.h"
#include "surface.h"
#include <vector>

namespace Tmpl8
{
	struct Tile
	{
		int sheetX; // tile's location in spritesheet space
		int sheetY;
		bool passthrough; // if player can pass through this tile
	};

	class Map
	{
	public:
		/**
		 * \brief gives info about a specific tile in the map
		 * \param tx  tile's x coordinate in screen-space
		 * \param ty  tile's y coordinate in screen-space
		 * \return a tuple of its info: x coordinate (tile-space), y coordinate (tile-space),the map's tile index in the spritesheet ,can the player go over the tile?
		 */
		std::tuple<int, int, int, bool> TileInfo(const int tx, const int ty);
		/**
		 * \brief Draws a single tile on screen
		 * \param tx  the tile's x coordinate (tile-space) in the spritesheet
		 * \param ty  the tile's y coordinate (tile-space) in the spritesheet
		 * \param screen  which surface to draw to
		 * \param x  the x offset (screen-space) of the top left corner
		 * \param y  the y offset (screen-space) of the top left corner
		 */
		void DrawTile(int tx, int ty, Surface* screen, int x, int y);
		/**
		 * \brief Draws all tiles on screen
		 * \param gameScreen  which surface to draw to
		 * \param startX  the map's left corner (screen-space)
		 * \param startY  the map's top corner (screen-space)
		 */
		void Map::RenderMap(Surface* gameScreen, const int startX, const int startY);

		Map() :
			mapSprite(new Surface("assets/mapTiles.png"), 1)
		{
		}
	private:
		Sprite mapSprite;
		const int mapWidth = 18, mapHeight = 6, mapParameters = 2;
	};
}
