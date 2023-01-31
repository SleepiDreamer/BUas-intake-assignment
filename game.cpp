#include "game.h"
#include "surface.h"

#include <iostream>
#include <windows.h>
#include <algorithm>

#include "bullet.h"
#include "map.h"
#include "util.h"


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
		lastShot += deltaTime; // increase shot timer
		const int ScreenWidth = screen->GetWidth(), ScreenHeight = screen->GetHeight();

		viewX1 += viewSpeedX * deltaTime; // move view window
		viewY1 += viewSpeedY * deltaTime;

		playerPosMid.x = playerPos.x + playerWidth / 2;
		playerPosMid.y = playerPos.y + playerHeight / 2;

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
		if (GetAsyncKeyState(VK_RIGHT)) playerSpeed.x = std::clamp(playerSpeed.x + playerSpeedAccel * deltaTime, -playerSpeedMax, playerSpeedMax);
		if (GetAsyncKeyState(VK_LEFT)) playerSpeed.x = std::clamp(playerSpeed.x - playerSpeedAccel * deltaTime, -playerSpeedMax, playerSpeedMax);
		if (GetAsyncKeyState(VK_UP)) playerSpeed.y = std::clamp(playerSpeed.y - playerSpeedAccel * deltaTime, -playerSpeedMax, playerSpeedMax);
		if (GetAsyncKeyState(VK_DOWN)) playerSpeed.y = std::clamp(playerSpeed.y + playerSpeedAccel * deltaTime, -playerSpeedMax, playerSpeedMax);

		// shoot
		if (GetAsyncKeyState(VK_LBUTTON) && lastShot > shotDelay)
		{
			lastShot = 0; // reset shot timer
			bullets.emplace_back(screen, playerPosMid.x, playerPosMid.y, RotationBetweenPoints(playerPosMid.x, playerPosMid.y, mousePos.x, mousePos.y), 1000.0f, 800.0f, 1, &bulletSprite);
		}


		// slow down when appropriate
		if (GetAsyncKeyState(VK_RIGHT) == GetAsyncKeyState(VK_LEFT) || (GetAsyncKeyState(VK_RIGHT) && playerSpeed.x < 0) || (GetAsyncKeyState(VK_LEFT) && playerSpeed.x > 0)) // slow down
			playerSpeed.x = playerSpeed.x -= (playerSpeed.x * 10.0f) * (deltaTime);
		if (GetAsyncKeyState(VK_UP) == GetAsyncKeyState(VK_DOWN) || (GetAsyncKeyState(VK_UP) && playerSpeed.y > 0) || (GetAsyncKeyState(VK_DOWN) && playerSpeed.y < 0))
			playerSpeed.y = playerSpeed.y -= (playerSpeed.y * 10.0f) * (deltaTime);

		// screen boundary collisions
		if (playerPos.x < 0 || playerPos.x + playerWidth > ScreenWidth) // TODO: fix so player can't go 1 or 2 pixels to the right of the screen anymore
		{
			playerSpeed.x = 0;
			playerPos.x = (ScreenWidth - playerWidth) * (playerPos.x > 0);
		}
		if (playerPos.y < 0 || playerPos.y + playerHeight > ScreenHeight) 
		{
			playerSpeed.y = 0;
			playerPos.y = (ScreenHeight - playerHeight) * (playerPos.y > 0);
		}

		// move player
		playerPos.x += playerSpeed.x * deltaTime;
		playerPos.y += playerSpeed.y * deltaTime;

		for (Enemy& enemy : enemies) {

			enemy.PointTowards(playerPosMid.x, playerPosMid.y);
			enemy.MoveTowards(playerPosMid.x, playerPosMid.y, 100 * deltaTime);
		}

		// move bullets

		for (Bullet& bullet : bullets) {
			//if (screen->(bullet.m_x, bullet.m_y, bullet.m_x + enemyWidth, bullet.m_y + enemyHeight, viewX1, viewY1, viewX2, viewY2))
			bullet.Update(deltaTime);
		}

		/* - rendering - */
		//backdrop.Draw(screen, 0, 0);
		screen->Clear(0);
		

		if (screen->Visibility(playerPos.x, playerPos.y, playerPos.x + playerWidth, playerPos.y + playerHeight, viewX1, viewY1, viewX2, viewY2))
			playerSprite.Draw(screen, playerPos.x, playerPos.y);

		for (Enemy& enemy : enemies) {
			if (screen->Visibility(enemy.GetPos().x, enemy.GetPos().y, enemy.GetPos().x + enemyWidth, enemy.GetPos().x + enemyHeight, viewX1, viewY1, viewX2, viewY2))
				enemy.Render(screen);
		}

		//screen->DrawView(viewX1, viewY1, viewX2, viewY2, 0x000000);
		screen->Box(viewX1, viewY1, viewX2, viewY2, 0xffffff);

		for (Bullet& bullet : bullets) {
			//if (screen->(bullet.m_x, bullet.m_y, bullet.m_x + enemyWidth, bullet.m_y + enemyHeight, viewX1, viewY1, viewX2, viewY2))
			bullet.Render(screen);
		}

		// bullet collisions
		for (Bullet& bullet : bullets) {
			for (Enemy& enemy : enemies)
				if (bullet.Collision(enemy))
				{
					std::cout << bullet.GetPos().x << std::endl;
					screen->Box(bullet.GetPos() - 1, bullet.GetPos() + 1, 0xff0000);
					screen->Box(enemy.GetPos() - 1, enemy.GetPos() + 1, 0x00ff00);

					//bullet.OnHit(enemy);
				}
		}

		//screen->Circle(playerPos.x, playerPos.y, 25, 0xffffff); //doesn't work
		//map.RenderMap(screen, 0, 0);

		// debug: draw box around ball with colors
		if (screen->Visibility(playerPos.x, playerPos.y, playerPos.x + playerWidth, playerPos.y + playerHeight, viewX1, viewY1, viewX2, viewY2) == 0)
			screen->Box(playerPos.x, playerPos.y, playerPos.x + playerWidth, playerPos.y + playerHeight, 0xff0000);
		else if (screen->Visibility(playerPos.x, playerPos.y, playerPos.x + playerWidth, playerPos.y + playerHeight, viewX1, viewY1, viewX2, viewY2) == 1)
			screen->Box(playerPos.x, playerPos.y, playerPos.x + playerWidth, playerPos.y + playerHeight, 0xffff00);
		else if (screen->Visibility(playerPos.x, playerPos.y, playerPos.x + playerWidth, playerPos.y + playerHeight, viewX1, viewY1, viewX2, viewY2) == 2)
			screen->Box(playerPos.x, playerPos.y, playerPos.x + playerWidth, playerPos.y + playerHeight, 0x00ff00);

	}
};