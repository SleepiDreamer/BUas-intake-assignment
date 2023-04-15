#include "game.h"
#include "surface.h"
#include "util.h"

#include <cstdlib>
#include <iostream>
#include <windows.h>
#include <algorithm>
#include <random>
#include <ctime>
#include <string>

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
		if (mouseLeftDown && mousePos.x > _pos1.x && mousePos.x < _pos2.x && mousePos.y > _pos1.y && mousePos.y < _pos2.y)
		{
			Shutdown();
		}
	}

	void Game::spawnEnemy()
	{
		vec2 pos;
		std::cout << "spawning enemy..." << std::endl;
		do {
			pos = { randfloat(-200, ScreenWidth + 200), randfloat(-200, ScreenHeight + 200) };
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
		bulletPool.clear();
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
		powerup = new Powerup({ 0, 0 }, screen, std::move(powerupSprite));
		bulletPool.init(screen, bulletSprite);
		enemyPool.init(screen, enemySprite);
	}

	// ----------------------------------------------------------- |
	// Initialize variables when play button is pressed			   |
	// ----------------------------------------------------------- |
	void Game::onStart()
	{
		player->setPos({ 500, 500 });
		player->setHp(player->getMaxHp());
		std::cout << "Started!" << std::endl;
		iButtonPressed = false;
		powerup->setActive(false);
		powerup->setLifetime(0.0f);
		powerup->setHP(powerup->getMaxHp());
		powerupSpeedTimer = 0.0f;
		powerupDamageTimer = 0.0f;
		powerupHealTimer = 0.0f;
		powerupSpawnTimer = 0.0f;
		powerupNukeTimer = 0.0f;
		powerupInvincibilityTimer = 0.0f;
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
		if (powerupSpeedTimer > 0.0f) dt *= 0.5f;
		
		const int screen_width = screen->GetWidth();
		const int screen_height = screen->GetHeight();

		if (gameRunning) {
			// ---*--- MOVEMENT ---*---
			player->update(dt);
			player->PointTowards(mousePos);

			// ---*--- ENEMIES ---*---
			enemyPool.update(player->getPos(), dt);
			enemySpawnTimer += dt;
			enemySpawnDelay = 15.0f / (time * 15.0f) + 0.65f;
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
				float damage = 10.0f;
				if (powerupDamageTimer > 0.0f) damage *= 3.0f;
				bulletPool.enable(player->getPos(), player->getDir() * 1000.0f , 1.0f, damage);
				player->resetShotTimer();
			}
			bulletPool.update(player->getPos(), dt);

			// ---*--- POWERUPS ---*---
			if (powerup->getActive())
			{
				powerup->setLifetime(powerup->getLifetime() - dt);
				powerup->setScale(powerup->getLifetime() / 6.0f);
				if (powerup->getLifetime() <= 0.0f)
				{
					powerup->setActive(false);
				}
			}
			else
			{
				powerupSpawnTimer += dt;
				if (powerupSpawnTimer > powerupSpawnDelay)
				{
					powerup->spawn();
					powerupSpawnTimer = 0.0f;
				}
			}
			powerupFireRateTimer -= dt;
			powerupDamageTimer -= dt;
			powerupSpeedTimer -= dt * 2;
			powerupInvincibilityTimer -= dt;
			powerupNukeTimer -= dt;
			powerupHealTimer -= dt;
			if (powerupFireRateTimer > 0.0f) player->setShotDelay(0.02f);
			else player->setShotDelay(0.05f);
			if (powerupSpeedTimer > 0.0f) dt *= 0.5f;
			else dt *= 2.0f;
			if (powerupNukeTimer == 2.0f)
			{
				score += bulletPool.getActiveBullets() * 50;
				std::cout << bulletPool.getActiveBullets() << std::endl;
				bulletPool.clear();
				std::cout << "nuke" << std::endl;
			}

			// ---*--- COLLISIONS ---*---
			for (int i = 0; i < bulletPool.getActiveBullets(); i++) 
			{
				Bullet* bullet = bulletPool.getPool()[i];
				bool stop = false;
				if (powerup->getActive()) {
					if (powerup->bulletCollisionCheck(bullet) == 1)
					{
						bulletPool.disable(bullet->getId());
						score += 10;
						stop = true;
					}
					else if (powerup->bulletCollisionCheck(bullet) == 2)
					{
						bulletPool.disable(bullet->getId());
						powerupSpawnTimer = 0.0f;
						powerup->consume();
						const int randPowerup = randint(0, 6);
						std::cout << "powerup: " << randPowerup << std::endl;
						const float duration = 6.0f;
						if		(randPowerup == 0) powerupDamageTimer = duration;				// damage
						else if (randPowerup == 1) powerupInvincibilityTimer = duration;		// invincibility
						else if (randPowerup == 3) {											// heal +1
							powerupHealTimer = 2.0f;
							player->setHp(min(player->getHp() + 1, 3)); 
						}				
						else if (randPowerup == 4) powerupNukeTimer = 2.0f;						// kill all enemies		
						else if (randPowerup == 5) powerupFireRateTimer = duration;				// fire rate
						else if (randPowerup == 6) powerupSpeedTimer = duration;				// slow down game
						score += 10;
						stop = true;
					}
				}
				if (enemyPool.bulletCollisionCheck(bullet) && !stop)
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
			powerup->render(screen);
			player->render();
			const char* scoreChar = stringToCString(std::to_string(score));
			screen->PrintScaled(scoreChar, 10, 10, 5, 5, 0xdddddd);
			delete[] scoreChar;
			for (int i = 0; i < player->getMaxHp(); i++)
			{
				const float xPos = static_cast<float>(ScreenWidth) / 2.0f + (static_cast<float>(i) - (player->getMaxHp() - 1) / 2.0f) * 80.0f;
				if (player->getHp() >= i + 1)
					heartSprite->DrawScaled(static_cast<int>(xPos), 50, 64, 64, screen, false);
				else
					heartSprite2->DrawScaled(static_cast<int>(xPos), 50, 64, 64, screen, false);
			}
			if	(powerupFireRateTimer > 0.0f) screen->CentreScaled("Faster gun!", ScreenHeight - 60, 5, 5, 0xffffff);
			else if (powerupDamageTimer > 0.0f) screen->CentreScaled("More damage!", ScreenHeight - 60, 5, 5, 0xffffff);
			else if (powerupSpeedTimer > 0.0f) screen->CentreScaled("Slower time!", ScreenHeight - 60, 5, 5, 0xffffff);
			else if (powerupInvincibilityTimer > 0.0f) screen->CentreScaled("Invincible!", ScreenHeight - 60, 5, 5, 0xffffff);
			else if (powerupNukeTimer > 0.0f) screen->CentreScaled("Nuked!", ScreenHeight - 60, 5, 5, 0xffffff);
			else if (powerupHealTimer > 0.0f) screen->CentreScaled("Healed!", ScreenHeight - 60, 5, 5, 0xffffff);


			// ---*--- DEATH MECHANIC ---*---
			time += dt;

			if (player->getInvincibility() <= 0.0f && enemyPool.playerCollisionCheck(player) && powerupInvincibilityTimer <= 0.0f)
			{
				if (player->playerDamaged())
				{
					gameRunning = false;
					//GameReset();
					gameOverTimer = 2.0f;
				}
			}
		}
		// ---*--- MENU SCREEN ---*---
		else if (gameOverTimer <= 0)
		{ // menu screen
			//screen->Line(ScreenWidth / 2, 0, ScreenWidth / 2, ScreenHeight, 0xffffff); // center line
			screen->CentreScaled("MR. BOUNCE", 40, 10, 10, 0xffffff);

			vec4 playButtonBox = { 545, 280, 730, 350 };
			playButton({ playButtonBox.x, playButtonBox.y }, { playButtonBox.z, playButtonBox.w });
			screen->Box(playButtonBox.x, playButtonBox.y, playButtonBox.z, playButtonBox.w, 0xffffff);
			screen->CentreScaled("PLAY", 300, 6, 6, 0xffffff);

			vec4 quitButtonBox = { 545, 380, 730, 450 };
			quitButton({ quitButtonBox.x, quitButtonBox.y }, { quitButtonBox.z, quitButtonBox.w });
			screen->Box(quitButtonBox.x, quitButtonBox.y, quitButtonBox.z, quitButtonBox.w, 0xffffff);
			screen->CentreScaled("QUIT", 400, 6, 6, 0xffffff);
		}
		// ---*--- GAME OVER SCREEN ---*---
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
