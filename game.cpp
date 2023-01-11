#include "game.h"
#include "surface.h"

#include <iostream>
#include <windows.h>
#include <algorithm>
#include "map.h"
 

namespace Tmpl8
{

	

	// -----------------------------------------------------------
	// Initialize the application
	// -----------------------------------------------------------
	void Game::Init()
	{
	}
	
	// -----------------------------------------------------------
	// Close down application
	// -----------------------------------------------------------
	void Game::Shutdown()
	{
	}


	// -----------------------------------------------------------
	// Main application tick function
	// -----------------------------------------------------------
	void Game::Tick(float deltaTime)
	{
		deltaTime = min(deltaTime / 1000.0f, 0.25f); // ms => s and clamp to 4 fps
		const int ScreenWidth = screen->GetWidth(), ScreenHeight = screen->GetHeight();

		viewX1 += viewSpeedX * deltaTime; // move view window
		viewY1 += viewSpeedY * deltaTime;

		const float viewX2 = viewX1 + viewWidth, viewY2 = viewY1 + viewHeight; // set view's x2 y2 coordinates

		// bounce view window
		if (viewX1 < 0 || viewX2 > static_cast<float>(ScreenWidth)) 
		{
			viewSpeedX *= -1;
			if (viewX1 < 0) viewX1 = 0;
			if (viewX2 > static_cast<float>(ScreenWidth)) viewX1 = static_cast<float>(ScreenWidth) - viewWidth;
		}
		if (viewY1 < 0 || viewY2 > static_cast<float>(ScreenHeight)) // bounce view window
		{
			viewSpeedY *= -1;
			if (viewY1 < 0) viewY1 = 0;
			if (viewY2 > static_cast<float>(ScreenHeight)) viewY1 = static_cast<float>(ScreenHeight) - viewHeight;
		}

		// add speed when pressing arrow keys
		if (GetAsyncKeyState(VK_RIGHT)) playerSpeedX = std::clamp(playerSpeedX + playerSpeedAccel * deltaTime, -playerSpeedMax, playerSpeedMax);
		if (GetAsyncKeyState(VK_LEFT)) playerSpeedX = std::clamp(playerSpeedX - playerSpeedAccel * deltaTime, -playerSpeedMax, playerSpeedMax);
		if (GetAsyncKeyState(VK_UP)) playerSpeedY = std::clamp(playerSpeedY - playerSpeedAccel * deltaTime, -playerSpeedMax, playerSpeedMax);
		if (GetAsyncKeyState(VK_DOWN)) playerSpeedY = std::clamp(playerSpeedY + playerSpeedAccel * deltaTime, -playerSpeedMax, playerSpeedMax);

		// slow down when appropriate
		if (GetAsyncKeyState(VK_RIGHT) == GetAsyncKeyState(VK_LEFT) || (GetAsyncKeyState(VK_RIGHT) && playerSpeedX < 0) || (GetAsyncKeyState(VK_LEFT) && playerSpeedX > 0)) // slow down
			playerSpeedX = playerSpeedX -= (playerSpeedX * 10.0f) * (deltaTime);
		if (GetAsyncKeyState(VK_UP) == GetAsyncKeyState(VK_DOWN) || (GetAsyncKeyState(VK_UP) && playerSpeedY > 0) || (GetAsyncKeyState(VK_DOWN) && playerSpeedY < 0))
			playerSpeedY = playerSpeedY -= (playerSpeedY * 10.0f) * (deltaTime);

		// screen boundary collisions
		if (playerX < 0 || playerX + playerWidth > ScreenWidth) // TODO: fix so player can't go 1 or 2 pixels to the right of the screen anymore
		{
			playerSpeedX = 0;
			playerX = (ScreenWidth - playerWidth) * (playerX > 0);
		}
		if (playerY < 0 || playerY + playerHeight > ScreenHeight) 
		{
			playerSpeedY = 0;
			playerY = (ScreenHeight - playerHeight) * (playerY > 0);
		}

		// move player
		playerX += playerSpeedX * deltaTime;
		playerY += playerSpeedY * deltaTime;

		for (Enemy& enemy : enemies) {

			enemy.PointTowards(playerX, playerY);
			enemy.MoveTowards(playerX, playerY, 100 * deltaTime);
		}

		/* - rendering - */
		backdrop.Draw(screen, 0, 0);
		

		if (screen->Visibility(playerX, playerY, playerX + playerWidth, playerY + playerHeight, viewX1, viewY1, viewX2, viewY2))
			playerSprite.Draw(screen, playerX, playerY);

		for (Enemy& enemy : enemies) {
			if (screen->Visibility(enemy.m_x, enemy.m_y, enemy.m_x + enemyWidth, enemy.m_y + enemyHeight, viewX1, viewY1, viewX2, viewY2))
				enemy.Render(screen);
		}

		screen->DrawView(viewX1, viewY1, viewX2, viewY2, 0x000000);
		screen->Box(viewX1, viewY1, viewX2, viewY2, 0xffffff);
		//map.RenderMap(screen, 0, 0);

		// debug: draw box around ball with colors
		if (screen->Visibility(playerX, playerY, playerX + playerWidth, playerY + playerHeight, viewX1, viewY1, viewX2, viewY2) == 0)
			screen->Box(playerX, playerY, playerX + playerWidth, playerY + playerHeight, 0xff0000);
		else if (screen->Visibility(playerX, playerY, playerX + playerWidth, playerY + playerHeight, viewX1, viewY1, viewX2, viewY2) == 1)
			screen->Box(playerX, playerY, playerX + playerWidth, playerY + playerHeight, 0xffff00);
		else if (screen->Visibility(playerX, playerY, playerX + playerWidth, playerY + playerHeight, viewX1, viewY1, viewX2, viewY2) == 2)
			screen->Box(playerX, playerY, playerX + playerWidth, playerY + playerHeight, 0x00ff00);
	}
};