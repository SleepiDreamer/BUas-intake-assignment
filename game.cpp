#include "game.h"
#include "surface.h"
#include "util.h"

#include <windows.h>
#include <algorithm>
#include <cassert>
#include <random>
#include <string>

float constexpr POWERUP_DURATION = 6.0f;
float constexpr POWERUP_MESSAGE_DURATION = 2.0f;

// TODO: dashes
// TODO: high score
// TODO: Transparency
// TODO: 

namespace Tmpl8
{
	/**
	 * \brief check pressed state for button
	 * \param _pos1 top left corner of button
	 * \param _pos2 bottom right corner of button
	 * \return whether button is pressed or not
	 */
	bool Game::buttonPressed(vec2 _pos1, vec2 _pos2) const
	{
		if (mouseLeftDown && mousePos.x > _pos1.x && mousePos.x < _pos2.x && mousePos.y > _pos1.y && mousePos.y < _pos2.y)
		{
			return true;
		}
		return false;
	}

	void Game::spawnEnemy()
	{
		vec2 spawnPos;
		do { // only spawn enemy outside of screen and not too close to player
			spawnPos = { randfloat(-200, ScreenWidth + 200), randfloat(-200, ScreenHeight + 200) };
		} while (insideScreen(spawnPos) || distanceBetween(player->getPos(), spawnPos) < 400.0f);
		enemyPool.enable(spawnPos, { 0, 0 }, 1.0f, 10.0f);
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
		gameRunning = true;
		bulletPool.clear();
		enemyPool.clear();
		time = 0;
		score = 0;
		player->setPos({ 500, 500 });
		player->setHp(player->getMaxHp());
		player->setInvincibility(0);
		iButtonPressed = false;
		powerup->setActive(false);
		powerup->setLifetime(0.0f);
		powerup->setHP(powerup->getMaxHp());
		powerupSpawnTimer = 0.0f;
		powerupTimer = 0.0f;
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
		if (powerupType == 4) dt *= 0.5f; // slow motion powerup
		
		const int screen_width = screen->GetWidth();
		const int screen_height = screen->GetHeight();

		// ________같같같같�________ |
		// ---*--- Main Game ---*--- |
		// --------같같같같�-------- |
		if (gameState == Playing) {
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
			bool canShoot;
			if (powerupType == 2) canShoot = player->canShootFast();
			else canShoot = player->canShoot();
			if (mouseLeftDown && canShoot)
			{
				float damage = 10.0f;
				if (powerupType == 1) damage *= 3.0f;
				bulletPool.enable(player->getPos(), player->getDir() * 1000.0f , 1.0f, damage);
				player->resetShotTimer();
			}
			bulletPool.update(player->getPos(), dt);

			// ---*--- POWERUPS ---*---
			if (powerup->getActive())
			{
				powerup->setLifetime(powerup->getLifetime() - dt);
				//powerup->setScale(powerup->getLifetime() / 6.0f);
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
			powerupTimer -= dt;
			if (powerupTimer <= 0.0f) powerupType = 0;
			if (powerupType == 4) powerupTimer -= dt; // make speed powerup not last longer
			if (powerupType == 1) player->setDamage(3.0f);

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
						powerupType = randint(1, 6);
						switch (powerupType)
						{
						case 1: powerupTimer = POWERUP_DURATION; break; // damage
						case 2: powerupTimer = POWERUP_DURATION; break; // fire rate
						case 3: powerupTimer = POWERUP_DURATION; break; // invincibility
						case 4: powerupTimer = POWERUP_DURATION; break; // speed
						case 5: powerupTimer = POWERUP_MESSAGE_DURATION; player->setHp(min(player->getHp() + 1, 3)); break; // health
						case 6: powerupTimer = POWERUP_MESSAGE_DURATION; enemyPool.clear(); score += enemyPool.getActiveEnemies() * 10; break; // nuke
						}
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
			switch (powerupType)
			{
			case 1: screen->CentreScaled("More damage", ScreenHeight - 60, 5, 5, 0xffffff); break;
			case 2: screen->CentreScaled("Faster gun", ScreenHeight - 60, 5, 5, 0xffffff); break;
			case 3: screen->CentreScaled("Invincibility", ScreenHeight - 60, 5, 5, 0xffffff); break;
			case 4: screen->CentreScaled("Slower time", ScreenHeight - 60, 5, 5, 0xffffff); break;
			case 5: screen->CentreScaled("Healed", ScreenHeight - 60, 5, 5, 0xffffff); break;
			case 6: screen->CentreScaled("Nuked", ScreenHeight - 60, 5, 5, 0xffffff); break;
			}
			screen->CentreBar(ScreenHeight - 20, ScreenHeight - 10, static_cast<int>((powerupTimer / POWERUP_DURATION) * 300.0f), 0xffffff);

			// ---*--- DEATH MECHANIC ---*---
			time += dt;

			if (player->getInvincibility() <= 0.0f && enemyPool.playerCollisionCheck(player) && powerupType != 3)
			{
				if (player->playerSubtractHealth()) // check if player is dead
				{
					gameRunning = false;
					gameOverTimer = 2.0f;
				}
			}
		}
		// --------같같같같같�-------- |
		// ---*--- MENU SCREEN ---*--- |
		// --------같같같같같�-------- |
		else if (gameState == MainMenu)
		{
			screen->CentreScaled("MR. BOUNCE", 40, 10, 10, 0xffffff);

			// play button
			vec4 playButtonBox = { 545, 280, 730, 350 };
			screen->Box({ playButtonBox.x, playButtonBox.y }, { playButtonBox.z, playButtonBox.w }, 0xffffff);
			screen->CentreScaled("PLAY", 300, 6, 6, 0xffffff);
			if (buttonPressed({ playButtonBox.x, playButtonBox.y }, { playButtonBox.z, playButtonBox.w })) onStart();

			// quit button
			vec4 quitButtonBox = { 545, 380, 730, 450 };
			screen->Box({ quitButtonBox.x, quitButtonBox.y }, { quitButtonBox.z, quitButtonBox.w }, 0xffffff);
			screen->CentreScaled("QUIT", 400, 6, 6, 0xffffff);
			if (buttonPressed({ quitButtonBox.x, quitButtonBox.y }, { quitButtonBox.z, quitButtonBox.w })) Shutdown();
		}
		// --------같같같같같같같같-------- |
		// ---*--- GAME OVER SCREEN ---*--- |
		// --------같같같같같같같같-------- |
		else if (gameState == Death)
		{
			gameOverTimer -= dt;
			if (gameOverTimer >= 0.0f)
			{
				screen->Clear(0);
				screen->CentreScaled("You Died!", 300, 6, 6, 0xffffff);
				screen->PrintScaled("Score: ", 460, 500, 6, 6, 0xffffff);
				const char* scoreChar = stringToCString(std::to_string(score));
				screen->PrintScaled(scoreChar, 690, 500, 6, 6, 0xffffff);
				delete[] scoreChar;
			}
			else
			{
				screen->Clear(0x000000);
				gameRunning = false;
			}
		}
		frame++;
	}
}
