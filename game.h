#pragma once
#include <cmath>
//#include <cstdio> //printf
#include <iostream>
#include <vector>

#include "bullet.h"
#include "enemy.h"
#include "map.h"
#include "surface.h"
#include "template.h"


namespace Tmpl8 {

	class Surface;

	


	class Game
	{
	public:
		void SetTarget( Surface* surface ) { screen = surface; }
		void Init();
		void Shutdown();
		void Tick( float deltaTime );
		void MouseUp( int button ) { /* implement if you want to detect mouse button presses */ }
		void MouseDown( int button ) { /* implement if you want to detect mouse button presses */ }
		void MouseMove(int x, int y)
		{
			mousePos.x = static_cast<float>(x);
			mousePos.y = static_cast<float>(y);
		}
		void KeyUp( int key ) { /* implement if you want to handle keys */ }
		void KeyDown( int key ) { /* implement if you want to handle keys */ }

		Game() :
			playerSprite(new Surface("assets/ball.png"), 1),
			backdrop(new Surface("assets/dogs.png"), 1),
			tankSprite(new Surface("assets/ctankbase.tga"), 16),
			bulletSprite(new Surface("assets/ball.png"), 1)
		{
			playerWidth = playerSprite.GetWidth();
			playerHeight = playerSprite.GetHeight();
			enemyWidth = tankSprite.GetWidth();
			enemyHeight = tankSprite.GetHeight();
			for (int i = 0; i < 8; i++) {
				enemies.push_back(std::move(Enemy({ 100.0f, static_cast<float>(i * 25) }, 0, 0, &tankSprite)));
			}
		}
		

	private:
		Surface* screen;

		vec2 mousePos;
		vec2 playerPos = { 200.0f, 200.0f };
		vec2 playerPosMid;
		const float playerSpeedMax = 450.0f;
		const float playerSpeedAccel = 2000.0f;
		vec2 playerSpeed = {0.0f, 0.0f};
		float playerWidth, playerHeight;
		float enemySpeed = 100.0f;
		float enemyWidth, enemyHeight;
		float lastShot = 0;
		float shotDelay = 0.03f;
		//float viewX1 = 100.0f, viewY1 = 100.0f, viewWidth = 700.0f, viewHeight = 350.0f, viewSpeedX = 100.0f, viewSpeedY = 100.0f;
		float viewX1 = 100.0f, viewY1 = 100.0f, viewWidth = 1280.0f, viewHeight = 720.0f, viewSpeedX = 100.0f, viewSpeedY = 100.0f;
		Sprite playerSprite;
		Sprite backdrop;
		Sprite tankSprite;
		Sprite bulletSprite;
		std::vector<Enemy> enemies;
		std::vector<Bullet> bullets;
		Map map;
	};
}; 