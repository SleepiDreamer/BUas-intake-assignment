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

		Game(Surface* surface);
		~Game();

	private:
		Surface* screen;

		Player* player;
		float moveWidth = 800.0f;
		float leftBound = (ScreenWidth - moveWidth) / 2;
		float rightBound = 800 + leftBound;

		vec2 mousePos;
		bool mouseLeftDown = false;

		bool run = false;
		Sprite* backdropSprite = new Sprite(new Surface("assets/backdrop.jpg"), 1);

		//PoolManager<Bullet, 300> bulletPool;
		BulletManager bulletPool = BulletManager(300);
		std::shared_ptr<Sprite> bulletSprite = std::make_shared<Sprite>(new Surface("assets/Bullet/bullet.png"), 1);
		//PoolManager<Enemy, 200> enemyPool;
		EnemyManager enemyPool = EnemyManager(200);
		std::shared_ptr<Sprite> enemySprite = std::make_shared<Sprite>(new Surface("assets/ctankbase.tga"), 16);
	};
}
