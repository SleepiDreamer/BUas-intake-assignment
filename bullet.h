#pragma once
#pragma once
#include "surface.h"
#include "template.h"
#include "util.h"
#include <cmath>
//#include <cstdio> //printf
#include <iostream>
#include <vector>

#include "enemy.h"


namespace Tmpl8 {

	class Surface;

	class Bullet
	{
	public:

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
		Bullet(Surface* _screen, float _x, float _y, float _rotation, vec2 _speed, float _damage, int _penetration, Sprite* _sprite) :
			m_sprite(_sprite)
		{
			m_screen = _screen;
			m_pos = { _x, _y };
			m_dir = vec2{cos(_rotation), sin(_rotation)};
			m_speed = _speed;
			m_damage = _damage;
			m_penetration = _penetration;
		}
		Bullet(Bullet&&) = default;
		~Bullet()
		{
			int i = 0;
		}


		void SetPos(const vec2& newPos);
		const vec2& GetPos() const;

		void SetDir(const vec2& newPos);
		const vec2& GetDir() const;

		void SetSpeed(const vec2& newPos);
		const vec2& GetSpeed() const;


		void Update(float deltaTime);

		void Render(Surface* gameScreen);

		bool Collision(const Enemy& enemy);

		void Delete();

		//void OnHit(const Enemy& enemy);


	private:
		Surface* m_screen;
		float m_damage;
		int m_penetration;
		vec2 m_pos, m_dir, m_speed;
		Sprite* m_sprite;
	};

};