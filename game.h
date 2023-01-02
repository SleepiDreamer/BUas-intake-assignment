#pragma once
#include "surface.h"
#include "template.h"
#include <cmath>
#include <cstdio> //printf
#include <iostream>
#include <vector>

namespace Tmpl8 {

class Surface;

class Enemy
{
public:

	float m_x, m_y, m_rotation, m_speed, m_frameDegrees;
	Sprite* m_sprite;
	int m_frame;
	Enemy(float _x, float _y, float _rotation, float _speed, Sprite* _sprite) :
		m_sprite(_sprite)
	{
		m_x = _x;
		m_y = _y;
		m_rotation = _rotation;
		m_speed = _speed;
		m_frameDegrees = 360 / m_sprite->Frames();
		m_frame = floor(static_cast<int>(m_rotation / m_frameDegrees) % m_sprite->Frames());
	}
	~Enemy()
	{
		int i = 0;
	}
	void Enemy::Move()
	{
		m_x++;
	}
	void Enemy::PointTowards(const float px, const float py)
	{
		m_rotation = atan2(py - m_y, px - m_x) + PI;
		m_frame = (unsigned int)(m_rotation / (2 * PI) * m_sprite->Frames()) % m_sprite->Frames();
	}

	void Enemy::MoveTowards(const float px, const float py, const float dist)
	{
		const float dir2p = atan2(py - m_y, px - m_x);
		m_x += cos(dir2p) * dist;
		m_y += sin(dir2p) * dist;
	}

	void Enemy::Render(Surface* gameScreen)
	{
		m_sprite->SetFrame(m_frame);
		m_sprite->Draw(gameScreen, m_x, m_y);
	}

};


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

};

}; // namespace Tmpl8