#include "game.h"
#include "surface.h"
#include "util.h"

#include <cstdlib>
#include <iostream>
#include <windows.h>
#include <algorithm>
#include <random>
#include <ctime>

namespace Tmpl8
{
	Game::Game(Surface* surface)
	{
		screen = surface;		
		//auto enemySprite = std::make_unique<Sprite>(new Surface("assets/ctankbase.tga"), 16);
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
		std::cout << "spawning enemy..." << std::endl;
		do {
			pos = { randfloat2(-200, ScreenWidth + 200), randfloat2(-200, ScreenHeight + 200) };
		} while (insideScreen(pos) || distanceBetween(player->getPos(), pos) < 400.0f);
		enemyPool.enable(pos, { 0, 0 }, 1.0f, 10.0f);
	}

	// ---------------------------------------------------------- |
	// Reset the game when the player dies                        |
	// ---------------------------------------------------------- |
	void Game::GameReset()
	{
		std::cout << "resetting game..." << std::endl;
		screen->Clear(0);
		for (int i = 0; i < bulletPool.getActiveBullets(); i++)
		{
			bulletPool.disable(enemyPool.getPool()[i]->getId());
		}
		enemyPool.clear();
		player->setInvincibility(0);
		time = 0;
		score = 0;
	}

	// ----------------------------------------------------------- |
	// Initialize the application								   |
	// ----------------------------------------------------------- |
	void Game::Init()
	{
		player = new Player({ 0, 0 }, screen, std::move(playerSprite));
		bulletPool.init(screen, bulletSprite);
		enemyPool.init(screen, enemySprite);
	}

	// ----------------------------------------------------------- |
	// Initialize variables when play button is pressed			   |
	// ----------------------------------------------------------- |
	void Game::onStart()
	{
		
		//enemyPool.enable({ 500, 0 }, { 0, 0 }, 1.0f, 10.0f);
		player->setPos({ 500, 500 });
		player->setHp(player->getMaxHp());
		std::cout << "Started!" << std::endl;
		iButtonPressed = false;
	}

	// ----------------------------------------------------------- |
	// Close down application									   |
	// ----------------------------------------------------------- |
	void Game::Shutdown()
	{
		// from MAX#2223 in #buas-intake on discord. https://discord.com/channels/515453022097244160/686661689894240277/1095673953734901761. Acquired on 12/04/2023
		SDL_Event user_event;
		user_event.type = SDL_QUIT;
		SDL_PushEvent(&user_event);
	}

	// ----------------------------------------------------------- |
	// Main application tick function							   |
	// ----------------------------------------------------------- |
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
			enemySpawnDelay = 15.0f / (time * 15.0f) + 0.65;
			if (enemySpawnTimer > enemySpawnDelay)
			{
				spawnEnemy();
				enemySpawnTimer = 0.0f;
			}
			
			if (GetAsyncKeyState(0x49)) // DEBUG: spawn enemy when 'i' is pressed
			{
				if (!iButtonPressed)
				{
					spawnEnemy();
					iButtonPressed = true;
				}
			}
			else
			{
				iButtonPressed = false;
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
				if (enemyPool.bulletCollisionCheck(bullet))
				{
					bulletPool.disable(bullet->getId());
					score += 10;
				}
			}

			// ---*--- RENDERING ---*---
			//backdrop->Draw(screen, 0, 0); // RENDER THIS FIRST!
			screen->Clear(0x363636);
			screen->BoxThicc(100, 100, ScreenWidth -100, ScreenHeight - 100, 5, 0xe75e5e);
			//screen->PrintScaled(floatToChar(enemySpawnDelay), 10, 10, 2, 2, 0x00ff00); // DEBUG
			bulletPool.render(screen);
			enemyPool.render(screen);
			player->render();
			screen->PrintScaled(floatToChar(round(time)), 900, 10, 5, 5, 0xdddddd);
			screen->PrintScaled(intToChar(score), 10, 10, 5, 5, 0xdddddd);
			for (int i = 0; i < player->getMaxHp(); i++)
			{
				const float xPos = static_cast<float>(ScreenWidth) / 2.0f + (static_cast<float>(i) - (player->getMaxHp() - 1) / 2.0f) * 80.0f;
				if (player->getHp() >= i + 1)
					heartSprite->DrawScaled(static_cast<int>(xPos), 50, 64, 64, screen, false);
				else
					heartSprite2->DrawScaled(static_cast<int>(xPos), 50, 64, 64, screen, false);
			}

			// ---*--- WTF DO I CALL THIS ---*---
			time += dt;

			if (player->getInvincibility() < 1.0f && enemyPool.playerCollisionCheck(player))
			{
				if (player->playerDamaged())
				{
					gameRunning = false;
					//GameReset();
					gameOverTimer = 2.0f;
				}
			}
		}
		else if (gameOverTimer <= 0)
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
		else if (gameOverTimer >= 0)
		{
			gameOverTimer -= dt;
			if (gameOverTimer >= 0.0f)
			{
				screen->Clear(0);
				screen->PrintScaled("You Died!", 500, 300, 6, 6, 0xffffff);
			}
			else
			{
				GameReset();
			}
		}
		frame++;
	}
}