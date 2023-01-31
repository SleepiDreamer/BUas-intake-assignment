#pragma once
#include "surface.h"
#include "template.h"
#include <cmath>
//#include <cstdio> //printf
#include <iostream>
#include <vector>

// TODO switch to set and get
// TODO use const vec2& and const member functions

namespace Tmpl8 {

	class Enemy
	{
	public:
		Enemy(const vec2& _pos, float _dir, float _speed, Sprite* _sprite) :
			m_sprite(_sprite)
		{
			m_pos = _pos;
			m_dir = _dir;
			m_speed = _speed;
			m_frameDegrees = 360 / m_sprite->Frames();
			//m_frame = floor(static_cast<int>(m_dir / m_frameDegrees) % m_sprite->Frames());
			m_frame = 0;
			m_width = _sprite->GetWidth();
			m_height = _sprite->GetHeight();
		}
		~Enemy()
		{
			int i = 0;
		}

		void SetPos(const vec2& newPos);
		const vec2& GetPos() const;

		void SetDir(const vec2& newPos);
		const vec2& GetDir() const;

		void SetSpeed(const vec2& newPos);
		const vec2& GetSpeed() const;


		/**
		 * \brief sets an enemy's rotation to point towards a coordinate
		 * \param px  x coordinate to point towards (screen-space)
		 * \param py  y coordinate to point towards (screen-space)
		 */
		void PointTowards(float px, float py);

		/**
		 * \brief moves the player towards a specified coordinate (screen-space)
		 * \param px  x coordinate to move towards
		 * \param py  y coordinate to move towards
		 * \param dist  how far to move
		 */
		void MoveTowards(const float px, const float py, const float dist);

		void Render(Surface* gameScreen);

	private:
		vec2 m_pos, m_dir, m_speed;
		float m_frameDegrees;
		Sprite* m_sprite;
		int m_frame, m_width, m_height;
	};
}