#pragma once
#include <cmath>
#include <iostream>
#include <array>
#include <SDL_events.h>
#include <SDL_scancode.h>

#include "enemy.h"
#include "template.h"
#include "player.h"
//#include "poolManager.h"
#include "bulletManager.h"
#include "bullet.h"
#include "enemyManager.h"

namespace Tmpl8
{
	class Surface;

	class Game
	{
	public:
		void SetTarget(Surface* surface) { screen = surface; }
		void GameReset();
		void Init();
		void Shutdown();
		void Tick(float deltaTime);
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

		void playButton(vec2 _pos1, vec2 _pos2);
		void quitButton(vec2 _pos1, vec2 _pos2);
		void onStart();
		void spawnEnemy();

		Game(Surface* surface);
		~Game();

	private:
		Surface* screen;

		vec2 mousePos;
		bool mouseLeftDown = false;
		bool iButtonPressed;

		int score = 0;
		int frame = 0;
		float time = 0;
		float enemySpawnTimer = 0;
		float enemySpawnDelay;
		float gameOverTimer = 0.0f;
		bool gameRunning = false;
		Sprite* backdrop = new Sprite(new Surface("assets/backdrop.jpg"), 1);

		std::unique_ptr<Sprite> heartSprite = std::make_unique<Sprite>(new Surface("assets/heart.png"), 1);
		std::unique_ptr<Sprite> heartSprite2 = std::make_unique<Sprite>(new Surface("assets/heart empty.png"), 1);
		Player* player;
		std::unique_ptr<Sprite> playerSprite = std::make_unique<Sprite>(new Surface("assets/ball.png"), 1);
		BulletManager bulletPool = BulletManager(300);
		std::shared_ptr<Sprite> bulletSprite = std::make_shared<Sprite>(new Surface("assets/Bullet/bullet.png"), 1);
		EnemyManager enemyPool = EnemyManager(200);
		std::shared_ptr<Sprite> enemySprite = std::make_shared<Sprite>(new Surface("assets/ctankbase.tga"), 16);
	};
}
