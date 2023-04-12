#include "game.h"
#include "surface.h"
#include "util.h"

#include <cstdlib>
#include <iostream>
#include <windows.h>
#include <algorithm>

namespace Tmpl8
{
	Game::Game(Surface* surface)
	{
		screen = surface;
		
		auto playerSprite = std::make_unique<Sprite>(new Surface("assets/ball.png"), 1);
		player = new Player({ 0, 0 }, screen, std::move(playerSprite));
		auto enemySprite = std::make_unique<Sprite>(new Surface("assets/ctankbase.tga"), 16);
		//enemy = new Enemy({500, 500}, screen, std::move(enemySprite));
	}
	Game::~Game()
	{
		delete player;

	}

	void Game::playButton(vec2 _pos1, vec2 _pos2)
	{
		if (mousePos.x > _pos1.x && mousePos.x < _pos2.x && mousePos.y > _pos1.y && mousePos.y < _pos2.y)
		{
			if (mouseLeftDown)
			{
				gameRunning = true;
				onStart();
				std::cout << "Button pressed" << std::endl;
			}
		}
	}
	void Game::quitButton(vec2 _pos1, vec2 _pos2)
	{
		if (mousePos.x > _pos1.x && mousePos.x < _pos2.x && mousePos.y > _pos1.y && mousePos.y < _pos2.y)
		{
			if (mouseLeftDown)
			{
				//exit(0);
				Shutdown();
			}
		}
	}

	void Game::spawnEnemy()
	{
		vec2 pos;
		std::cout << "yes" << std::endl;
		do {
			pos = { randfloat(-200, ScreenWidth + 200), randfloat(-200, ScreenHeight + 200) };
		} while (insideScreen(pos) && distanceBetween(player->getPos(), pos) < 400.0f);
		enemyPool.enable(pos, { 0, 0 }, 1.0f, 10.0f);
	}

	// ----------------------------------------------------------- //
	// Initialize the application								   //
	// ----------------------------------------------------------- //
	void Game::Init()
	{
		// this function feels empty :'(
	}

	// ----------------------------------------------------------- //
	// Initialize variables when play button is pressed			   //
	// ----------------------------------------------------------- //
	void Game::onStart()
	{
		bulletPool.init(screen, bulletSprite);
		enemyPool.init(screen, enemySprite);
		//enemyPool.enable({ 500, 0 }, { 0, 0 }, 1.0f, 10.0f);
		player->setPos({ 500, 500 });
		spawnEnemy();
		std::cout << "Started!" << std::endl;
	}

	// ----------------------------------------------------------- //
	// Close down application									   //
	// ----------------------------------------------------------- //
	void Game::Shutdown()
	{
		// from MAX#2223 in #buas-intake on discord. https://discord.com/channels/515453022097244160/686661689894240277/1095673953734901761. Acquired on 12/04/2023
		SDL_Event user_event;
		user_event.type = SDL_QUIT;
		SDL_PushEvent(&user_event);
	}

	// ----------------------------------------------------------- //
	// Main application tick function							   //
	// ----------------------------------------------------------- //
	void Game::Tick(float dt)
	{
		//dt = deltaTime
		dt = min(dt / 1000.0f, 0.1f); // ms => s and clamp to minimum 10 fps
		
		const int screen_width = screen->GetWidth();
		const int screen_height = screen->GetHeight();

		if (gameRunning) {
			// ---*--- MOVEMENT ---*---
			player->update(dt);
			player->PointTowards(mousePos);

			// ---*--- ENEMIES ---*---
			enemyPool.update(player->getPos(), dt);
			enemySpawnTimer += dt;
			if (enemySpawnTimer > enemySpawnDelay)
			{
				spawnEnemy();
				enemySpawnTimer = 0.0f;
			}

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
			//backdrop->Draw(screen, 0, 0); // RENDER THIS FIRST!
			screen->Clear(0);
			bulletPool.render(screen);
			enemyPool.render(screen);
			player->render();
		}
		else
		{ // menu screen
			//screen->Line(ScreenWidth / 2, 0, ScreenWidth / 2, ScreenHeight, 0xffffff); // center line
			screen->PrintScaled("MR. BOUNCE", 350, 40, 10, 10, 0xffffff);

			vec4 playButtonBox = { 550, 280, 730, 350 };
			playButton({ playButtonBox.x, playButtonBox.y }, { playButtonBox.z, playButtonBox.w });
			screen->Box(playButtonBox.x, playButtonBox.y, playButtonBox.z, playButtonBox.w, 0xffffff);
			screen->PrintScaled("PLAY", 570, 300, 6, 6, 0xffffff);

			vec4 quitButtonBox = { 550, 380, 730, 450 };
			quitButton({ quitButtonBox.x, quitButtonBox.y }, { quitButtonBox.z, quitButtonBox.w });
			screen->Box(quitButtonBox.x, quitButtonBox.y, quitButtonBox.z, quitButtonBox.w, 0xffffff);
			screen->PrintScaled("QUIT", 570, 400, 6, 6, 0xffffff);
		}
		frame++;
		time += dt;
	}
}