#pragma once
#include <cmath>
//#include <cstdio> //printf
#include <iostream>
#include <vector>

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
		void MouseMove( int x, int y ) { /* implement if you want to detect mouse movement */ }
		void KeyUp( int key ) { /* implement if you want to handle keys */ }
		void KeyDown( int key ) { /* implement if you want to handle keys */ }

		Game() :
			playerSprite(new Surface("assets/ball.png"), 1),
			backdrop(new Surface("assets/dogs.png"), 1),
			tankSprite(new Surface("assets/ctankbase.tga"), 16)
		{
			playerWidth = playerSprite.GetWidth();
			playerHeight = playerSprite.GetHeight();
			enemyWidth = tankSprite.GetWidth();
			enemyHeight = tankSprite.GetHeight();
			for (int i = 0; i < 8; i++) {
				enemies.push_back(std::move(Enemy(100, i*25, 0, 0, &tankSprite)));
			}
		}
		

	private:
		Surface* screen;

		float playerX = 200.0f, playerY = 200.0f;
		const float playerSpeedMax = 450.0f, playerSpeedAccel = 2000.0f;
		float playerSpeedX = 0.0f, playerSpeedY = 0.0f;
		float playerWidth, playerHeight;
		float enemySpeed = 100.0f;
		float enemyWidth, enemyHeight;
		float viewX1 = 100.0f, viewY1 = 100.0f, viewWidth = 700.0f, viewHeight = 350.0f, viewSpeedX = 100.0f, viewSpeedY = 100.0f;
		Sprite playerSprite;
		Sprite backdrop;
		Sprite tankSprite;
		std::vector<Enemy> enemies;
		Map map;
	};
}; 