#include "game.h"
#include "surface.h"
#include "util.h"

#include <iostream>
#include <windows.h>
#include <algorithm>
#include <SDL_events.h>

namespace Tmpl8
{
	Game::Game(Surface* surface)
	{
		screen = surface;
		
		auto playerSprite = std::make_unique<Sprite>(new Surface("assets/Character cropped/spritesheet.png"), 12);
		player = new Player({ 200, 200 }, screen, std::move(playerSprite));
		auto enemySprite = std::make_unique<Sprite>(new Surface("assets/ctankbase.tga"), 16);
		//enemy = new Enemy({500, 500}, screen, std::move(enemySprite));
	}
	Game::~Game()
	{
		delete player;
	}

	

	// -----------------------------------------------------------
	// Initialize the application
	// -----------------------------------------------------------
	void Game::Init()
	{
		bulletPool.init(screen, bulletSprite);
		enemyPool.init(screen, enemySprite);
		enemyPool.enable({ 500, 500 }, { 0, 0 }, 1.0f, 10.0f);
	}

	// -----------------------------------------------------------
	// Close down application
	// -----------------------------------------------------------
	void Game::Shutdown()
	{}

	// -----------------------------------------------------------
	// Main application tick function
	// -----------------------------------------------------------
	void Game::Tick(float dt)
	{
		//dt = deltaTime
		dt = min(dt / 1000.0f, 0.1f); // ms => s and clamp to minimum 10 fps
		
		const int screen_width = screen->GetWidth();
		const int screen_height = screen->GetHeight();

		if (run) {
			// ---*--- MOVEMENT ---*---
			player->update(dt);
			player->PointTowards(mousePos);

			// ---*--- ENEMIES ---*---
			enemyPool.update(player->getPos(), dt);

			// ---*--- BULLETS ---*---
			if (mouseLeftDown && player->canShoot())
			{
				bulletPool.enable(player->getPos(), player->getDir() * 1000.0f , 1.0f, 10.0f);
				player->resetShotTimer();
			}
			bulletPool.update(player->getPos(), dt);

			// ---*--- COLLISIONS ---*---
			for (int i = 0; i < bulletPool.getActiveBullets(); i++) 
			{
				Bullet* bullet = bulletPool.getPool()[i];
				if (enemyPool.collisionCheck(bullet))
				{
					bulletPool.disable(i);
				}
			}

			// ---*--- RENDERING ---*---
			backdrop->Draw(screen, 0, 0); // RENDER THIS FIRST!
			bulletPool.render(screen);
			enemyPool.render(screen);
			player->render();
		}
		else
		{
			//screen->Line(ScreenWidth / 2, 0, ScreenWidth / 2, ScreenHeight, 0xffffff);
			screen->PrintScaled("MR. BOUNCE", 350, 40, 10, 10, 0xffffff); // center text
			screen->PrintScaled("PLAY", 570, 300, 6, 6, 0xffffff);
			screen->PrintScaled("QUIT", 570, 400, 6, 6, 0xffffff);
			if (mouseLeftDown)
			{
				run = true;
			}
		}
	}
}