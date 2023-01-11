#pragma once
#pragma once
#include "surface.h"
#include "template.h"
#include <cmath>
//#include <cstdio> //printf
#include <iostream>
#include <vector>


namespace Tmpl8 {

	class Surface;

	class Bullet
	{
	public:

		float m_x, m_y, m_rotation, m_speed, m_damage;
		int m_penetration;
		Sprite* m_sprite;

		/**
		 * \brief
		 * \param _x  initial x coordinate
		 * \param _y  initial y coordinate
		 * \param _rotation  initial rotation in radians
		 * \param _speed  initial speed in px/s
		 * \param _damage  damage
		 * \param _penetration  how many times the bullet can go through an enemy
		 * \param _sprite  bullet's sprite
		 */
		Bullet(float _x, float _y, float _rotation, float _speed, float _damage, int _penetration, Sprite* _sprite) :
			m_sprite(_sprite)
		{
			m_x = _x;
			m_y = _y;
			m_rotation = _rotation;
			m_speed = _speed;
			m_damage = _damage;
			m_penetration = _penetration;
		}
		~Bullet()
		{
			int i = 0;
		}


		void Bullet::Move()
		{
			m_x += cos(m_rotation);
			m_y += sin(m_rotation);
		}

		void Bullet::Render(Surface* gameScreen)
		{
			m_sprite->Draw(gameScreen, m_x, m_y);
		}
	};

};