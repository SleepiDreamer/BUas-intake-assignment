#pragma once
#include <iostream>
#include <array>
#include <SDL_events.h>

#include "template.h"
#include "player.h"
#include "powerup.h"
#include "bulletManager.h"
#include "enemyManager.h"
#include "particleManager.h"

namespace Tmpl8
{
	class Surface;

	class Game
	{
	public:
		void SetTarget(Surface* surface) { screen = surface; }
		void Init();
		static void Shutdown();
		void Tick(float dt);
		void MouseUp(int button) { 
			if (button == SDL_BUTTON_LEFT)
			{
				mouseLeftDown = false;
			}
		}
		void MouseDown(int button)
		{
			if (button == SDL_BUTTON_LEFT)
			{
				mouseLeftDown = true;
			}
		}
		void MouseMove(int x, int y)
		{
			mousePos.x = static_cast<float>(x);
			mousePos.y = static_cast<float>(y);
		}
		void KeyUp(int key) { /* implement if you want to handle keys */ }
		void KeyDown(int key) { /* implement if you want to handle keys */ }

		bool buttonPressed(vec2 _pos1, vec2 _pos2) const;
		void onStart();
		void spawnEnemy();

		Game(Surface* surface) // constructor
		{
			screen = surface;
			player = new Player({ 0, 0 }, screen, std::move(playerSprite));
			powerup = new Powerup({ 0, 0 }, screen, std::move(powerupSprite));
		}
		~Game() // destructor
		{
			delete player;
			delete powerup;
		}

	private:
		Surface* screen;

		vec2 mousePos;
		bool mouseLeftDown = false;
		bool iButtonPressed = false;

		enum gameStates { MainMenu, Playing, Death, Instructions };
		gameStates gameState = MainMenu;

		int score = 0;
		int highScore = 0;
		int frame = 0;
		float time = 0.0f;
		float enemySpawnTimer = 0.0f;
		float enemySpawnDelay = 1.0f;
		float gameOverTimer = 0.0f;
		float powerupSpawnTimer = 0.0f;
		float powerupSpawnDelay = 6.0f;

		float powerupTimer = 0.0f;
		int powerupType = 0;

		Sprite* backdrop = new Sprite(new Surface("assets/backdrop.jpg"), 1);

		std::unique_ptr<Sprite> heartSprite = std::make_unique<Sprite>(new Surface("assets/heart.png"), 1);
		std::unique_ptr<Sprite> heartSprite2 = std::make_unique<Sprite>(new Surface("assets/heart empty.png"), 1);
		Player* player;
		std::unique_ptr<Sprite> playerSprite = std::make_unique<Sprite>(new Surface("assets/ball.png"), 1);
		BulletManager bulletPool = BulletManager(150);
		std::shared_ptr<Sprite> bulletSprite = std::make_shared<Sprite>(new Surface("assets/Bullet/bullet.png"), 1);
		EnemyManager enemyPool = EnemyManager(150);
		std::shared_ptr<Sprite> enemySprite = std::make_shared<Sprite>(new Surface("assets/ctankbase.tga"), 16);
		ParticleManager particlePool = ParticleManager(600);
		std::shared_ptr<Sprite> particleSprite = std::make_shared<Sprite>(new Surface("assets/ball.png"), 1);
		std::unique_ptr<Sprite> powerupSprite = std::make_unique<Sprite>(new Surface("assets/powerup.png"), 1);
		Powerup* powerup;
	};
}
