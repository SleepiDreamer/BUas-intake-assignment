#include "game.h"
#include "surface.h"
#include "util.h"

#include <windows.h>
#include <fstream>
#include <cassert>
#include <random>
#include <string>

float constexpr POWERUP_DURATION = 6.0f;
float constexpr POWERUP_MESSAGE_DURATION = 2.0f;
unsigned int constexpr BG_COLOR = 0x464646;
unsigned int constexpr MENU_BG_COLOR = 0x000000;

// TODO: use enums for powerupType and particleType

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
		bulletPool.init(screen, bulletSprite);
		enemyPool.init(screen, enemySprite);
		particlePool.init(screen, particleSprite);
		highScore = stoi(readNthLine("data.txt", 0));
	}

	// ----------------------------------------------------------- |
	// Initialize variables when play button is pressed			   |
	// ----------------------------------------------------------- |
	void Game::onStart()
	{
		gameState = playing;
		bulletPool.clear();
		enemyPool.clear();
		particlePool.clear();
		time = 0;
		score = 0;
		scoreDisplay = 0;
		isPersonalBest = false;
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
		SDL_Event userEvent;
		userEvent.type = SDL_QUIT;
		SDL_PushEvent(&userEvent);
	}

	// ----------------------------------------------------------- |
	// Main application tick function							   |
	// ----------------------------------------------------------- |
	void Game::Tick(float dt)
	{
		//dt = deltaTime
		dt = min(dt / 1000.0f, 0.1f); // ms => s and clamp to minimum 10 fps
		if (powerupType == 4) dt *= 0.5f; // slow motion powerup

		// ________~~~~~~~~~________ |
		// ---*--- Main Game ---*--- |
		// --------~~~~~~~~~-------- |
		if (gameState == playing) {
			// ---*--- MOVEMENT ---*---
			player->update(dt);
			player->PointTowards(mousePos);

			// ---*--- ENEMIES ---*---
			enemyPool.update(player->getPos(), dt);
			enemySpawnTimer += dt;
			enemySpawnDelay = 15.0f / (time * 5.0f) + 0.35f; // increase spawn rate over time
			if (enemySpawnTimer > enemySpawnDelay)
			{
				spawnEnemy();
				enemySpawnTimer = 0.0f;
			}
			
			if (GetAsyncKeyState(0x49)) // DEBUG: spawn enemy manually when 'i' is pressed
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
			// if 'o' pressed
			if (GetAsyncKeyState(0x4F))
			{
				gameOverTimer = 2.0f;
				gameState = death;
			}

			// ---*--- BULLETS ---*---
			bool canShoot;
			if (powerupType == 2) canShoot = player->canShootFast();
			else canShoot = player->canShoot();
			if (mouseLeftDown && canShoot)
			{
				float damage = 20.0f;
				if (powerupType == 1) damage *= 3.0f;
				const float playerDir = atan2(player->getDir().x, player->getDir().y);
				const float shootDir = -playerDir + randfloat(-0.03f, 0.03f) + PI / 2.0f;
				const vec2 shootVel = { cos(shootDir) * 1000.0f, sin(shootDir) * 1000.0f };
				bulletPool.enable(player->getPos(), shootVel , 1.0f, damage);
				player->resetShotTimer();
			}
			bulletPool.update(player->getPos(), dt);

			// ---*--- PARTICLES ---*---
			particlePool.update(dt);

			// ---*--- POWERUPS ---*---
			powerup->update(dt);
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
						powerup->setWhiteFlashTimer(1.0f);
						bulletPool.disable(bullet->getId());
						score += 10;
						stop = true;
					}
					else if (powerup->bulletCollisionCheck(bullet) == 2)
					{
						bulletPool.disable(bullet->getId());
						powerupSpawnTimer = 0.0f;
						particlePool.powerupConsumed(powerup->getPos());
						powerup->consume();
						do { powerupType = randint(1, 6); } while (powerupType == 5 && player->getHp() == player->getMaxHp()); // don't heal if already full health
						switch (powerupType)
						{
						case 1: powerupTimer = POWERUP_DURATION; break; // damage
						case 2: powerupTimer = POWERUP_DURATION; break; // fire rate
						case 3: powerupTimer = POWERUP_DURATION; break; // invincibility
						case 4: powerupTimer = POWERUP_DURATION; break; // speed
						case 5: powerupTimer = POWERUP_MESSAGE_DURATION; particlePool.playerHealed(player->getPos());  player->setHp(min(player->getHp() + 1, 3)); break; // health
						case 6: powerupTimer = POWERUP_MESSAGE_DURATION; particlePool.clear(); enemyPool.clear(); score += enemyPool.getActiveEnemies() * 10; break; // nuke
						}
						score += 10;
						stop = true;
					}
				}
				const int bulletCollision = enemyPool.bulletCollisionCheck(bullet);
				if (bulletCollision == 1 && !stop)
				{
					bulletPool.disable(bullet->getId());
					score += 10;
				}
				else if (bulletCollision == 2 && !stop)
				{
					particlePool.enemyDied(bullet->getPos());
					bulletPool.disable(bullet->getId());
					score += 10;
				}
			}

			// ---*--- RENDERING ---*---
			//backdrop->Draw(screen, 0, 0); 
			screen->Clear(BG_COLOR); // RENDER THIS FIRST! (stupid me keeps forgetting)
			screen->BarShadow({ 100 - 5, 100 + 5 }, { 105 - 5, ScreenHeight - 100 + 5}, 10.0f, 0.15f); // left red line shadows
			screen->BarShadow({ 100 - 5, 100 + 5 }, { ScreenWidth - 100 - 5, 105 + 5 }, 10.0f, 0.15f); // top red line shadows
			screen->BarShadow({ ScreenWidth - 105 - 5, 100 + 5 }, { ScreenWidth - 100 - 5, ScreenHeight - 100 + 5 }, 10.0f, 0.15f); // right red line shadows
			screen->BarShadow({ 100 - 5, ScreenHeight - 105 + 5 }, { ScreenWidth - 100 - 5, ScreenHeight - 100 + 5 }, 10.0f, 0.15f); // bottom red line shadows
			screen->BoxThick(100, 100, ScreenWidth -100, ScreenHeight - 100, 5, 0xe75e5e); // red line around screen

			player->renderDirectionLine();
			particlePool.render(screen);
			powerup->render(screen);
			bulletPool.render(screen);
			enemyPool.render(screen);

			player->render();
			screen->VignetteFast(0.25f);
			scoreDisplay = (static_cast<float>(score) * 0.1f + scoreDisplay + 0.1f) / 1.1f;
			screen->PrintScaled(("Score: " + std::to_string(static_cast<int>(scoreDisplay))).c_str(), 10, 10, 5, 5, 0xdddddd);
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
			case 1: screen->PrintCentreScaled("Damage boost", ScreenHeight - 60, 5, 5, 0xffffff); break;
			case 2: screen->PrintCentreScaled("Faster shooting", ScreenHeight - 60, 5, 5, 0xffffff); break;
			case 3: screen->PrintCentreScaled("Invincibility", ScreenHeight - 60, 5, 5, 0xffffff); break;
			case 4: screen->PrintCentreScaled("Slow-motion", ScreenHeight - 60, 5, 5, 0xffffff); break;
			case 5: screen->PrintCentreScaled("Healed", ScreenHeight - 60, 5, 5, 0xffffff); break;
			case 6: screen->PrintCentreScaled("Nuked", ScreenHeight - 60, 5, 5, 0xffffff); break;
			}
			screen->BarCentre(ScreenHeight - 20, ScreenHeight - 10, static_cast<int>(powerupTimer / POWERUP_DURATION * 300.0f), 0xffffff);
			if (powerupType == 6) screen->Bar(0, 0, ScreenWidth - 1, ScreenHeight - 1, 0xffffff, powerupTimer / POWERUP_MESSAGE_DURATION);

			// ---*--- DEATH MECHANIC ---*---
			time += dt;

			if (player->getInvincibility() <= 0.0f && enemyPool.playerCollisionCheck(player) && powerupType != 3)
			{
				if (player->playerSubtractHealth()) // check if player is dead
				{
					gameState = death;
					gameOverTimer = 2.0f;
					writeToFile("data.txt", max(score, highScore));
					if (score > highScore) isPersonalBest = true;
					highScore = max(score, highScore);
				}
			}
		}
		// --------~~~~~~~~~~~-------- |
		// ---*--- MENU SCREEN ---*--- |
		// --------~~~~~~~~~~~-------- |
		else if (gameState == mainMenu)
		{
			screen->Clear(MENU_BG_COLOR);
			screen->PrintCentreScaled("SIR BOUNCE", 40, 10, 10, 0xffffff);
			screen->PrintCentreScaled(("High score: " + std::to_string(highScore)).c_str(), ScreenHeight - 50, 3, 3, 0xffffff);

			// play button
			vec4 playButtonBox = { 545, 180, 730, 250 };
			screen->Box({ playButtonBox.x, playButtonBox.y }, { playButtonBox.z, playButtonBox.w }, 0xffffff);
			screen->PrintCentreScaled("PLAY", 200, 6, 6, 0xffffff);
			if (buttonPressed({ playButtonBox.x, playButtonBox.y }, { playButtonBox.z, playButtonBox.w })) onStart();

			// instructions button
			vec4 instructionsButtonBox = { 545, 280, 730, 350 };
			screen->Box({ instructionsButtonBox.x, instructionsButtonBox.y }, { instructionsButtonBox.z, instructionsButtonBox.w }, 0xffffff);
			screen->PrintCentreScaled("HELP", 300, 6, 6, 0xffffff);
			if (buttonPressed({ instructionsButtonBox.x, instructionsButtonBox.y }, { instructionsButtonBox.z, instructionsButtonBox.w })) gameState = instructions;

			// quit button
			vec4 quitButtonBox = { 545, 380, 730, 450 };
			screen->Box({ quitButtonBox.x, quitButtonBox.y }, { quitButtonBox.z, quitButtonBox.w }, 0xffffff);
			screen->PrintCentreScaled("QUIT", 400, 6, 6, 0xffffff);
			if (buttonPressed({ quitButtonBox.x, quitButtonBox.y }, { quitButtonBox.z, quitButtonBox.w })) Shutdown();

		}
		// --------~~~~~~~~~~~~~~~~~~~-------- |
		// ---*--- INSTRUCTIONS SCREEN ---*--- |
		// --------~~~~~~~~~~~~~~~~~~~-------- |
		else if (gameState == instructions)
		{
			screen->Clear(MENU_BG_COLOR);
			screen->PrintScaled("Instructions", 25, 25, 6, 6, 0xffffff);
			screen->PrintScaled("You move automatically!", 25, 100, 3, 3, 0xffffff);
			screen->PrintScaled("You must shoot the enemies 3 times to kill them.", 25, 150, 3, 3, 0xffffff);
			screen->PrintScaled("Powerups spawn randomly on the map. Shoot them to get a surprise!", 25, 200, 3, 3, 0xffffff);
			screen->PrintScaled("-Damage Boost: multiplies all damage dealt by 3x (insta kill!)", 25, 250, 3, 3, 0xffffff);
			screen->PrintScaled("-Faster shooting: multiplies your fire rate by 2x", 25, 300, 3, 3, 0xffffff);
			screen->PrintScaled("-Invincibility: prevents you from getting hit", 25, 350, 3, 3, 0xffffff);
			screen->PrintScaled("-Slow-motion: time is twice as slow", 25, 400, 3, 3, 0xffffff);
			screen->PrintScaled("-Heal: heals you for an extra health point", 25, 450, 3, 3, 0xffffff);
			screen->PrintScaled("-Nuke: kills all enemies on screen", 25, 500, 3, 3, 0xffffff);
			screen->PrintScaled("Try to beat my personal top score of 23070!", 25, 600, 3, 3, 0xffffff);
			screen->PrintScaled("Good luck!", 25, 650, 3, 3, 0xffffff);

			// menu button
			vec4 menuButtonBox = { 1045, 25, 1230, 95 };
			screen->Box({ menuButtonBox.x, menuButtonBox.y }, { menuButtonBox.z, menuButtonBox.w }, 0xffffff);
			screen->PrintScaled("MENU", 1070, 45, 6, 6, 0xffffff);
			if (buttonPressed({ menuButtonBox.x, menuButtonBox.y }, { menuButtonBox.z, menuButtonBox.w })) { gameState = mainMenu; screen->Clear(0); }
		}
		// --------~~~~~~~~~~~~~~~~-------- |
		// ---*--- GAME OVER SCREEN ---*--- |
		// --------~~~~~~~~~~~~~~~~-------- |
		else if (gameState == death)
		{
			gameOverTimer -= dt;
			if (gameOverTimer >= 0.0f)
			{
				screen->Clear(MENU_BG_COLOR);
				screen->PrintCentreScaled("You Died!", 150, 6, 6, 0xffffff);
				screen->PrintCentreScaled(("Score: " + std::to_string(score)).c_str(), ScreenHeight / 2, 6, 6, 0xffffff);
				screen->PrintCentreScaled(("High score: " + std::to_string(highScore)).c_str(), ScreenHeight / 2 + 60, 4, 4, 0xffffff);
				if (isPersonalBest) screen->PrintCentreScaled("New personal best!", ScreenHeight / 2 + 110, 4, 4, 0xffffff);
			}
			else
			{
				screen->Clear(MENU_BG_COLOR);
				gameState = mainMenu;
			}
		}
		frame++;
	}
}
