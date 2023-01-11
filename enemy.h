#pragma once
#include "surface.h"
#include "template.h"
#include <cmath>
//#include <cstdio> //printf
#include <iostream>
#include <vector>

namespace Tmpl8 {

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

		/**
		 * \brief sets an enemy's rotation to point towards a coordinate
		 * \param px  x coordinate to point towards (screen-space)
		 * \param py  y coordinate to point towards (screen-space)
		 */
		void Enemy::PointTowards(const float px, const float py)
		{
			m_rotation = atan2(py - m_y, px - m_x) + PI;
			m_frame = (unsigned int)(m_rotation / (2 * PI) * m_sprite->Frames()) % m_sprite->Frames();
		};
		/**
		 * \brief moves the player towards a specified coordinate (screen-space)
		 * \param px  x coordinate to move towards
		 * \param py  y coordinate to move towards
		 * \param dist  how far to move
		 */
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
}