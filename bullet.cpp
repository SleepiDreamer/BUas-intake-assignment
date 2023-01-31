#include "bullet.h"
#include "enemy.h"
#include "util.h"

namespace Tmpl8
{
	void Bullet::SetPos(const vec2& newPos)
	{
		m_pos = newPos;
	}
	const vec2& Bullet::GetPos() const
	{
		return m_pos;
	}

	void Bullet::SetDir(const vec2& newDir)
	{
		m_dir = newDir;
	}
	const vec2& Bullet::GetDir() const
	{
		return m_dir;
	}

	void Bullet::SetSpeed(const vec2& newSpeed)
	{
		m_speed = newSpeed;
	}
	const vec2& Bullet::GetSpeed() const
	{
		return m_speed;
	}


	void Bullet::Update(float deltaTime)
	{
		m_pos += m_dir * m_speed * deltaTime;
		//m_speed *= 0.99; // TODO: Dampening
	}


	void Bullet::Render(Surface* gameScreen)
	{
		//m_sprite->Draw(gameScreen, static_cast<int>(m_pos.x), static_cast<int>(m_pos.y));
		m_screen->Box(m_pos.x - 1, m_pos.y - 1, m_pos.x + 1, m_pos.y + 1, 0xffffff);
	}

	bool Bullet::Collision(const Enemy& enemy)
	{
		return distanceBetween(m_pos, enemy.GetPos()) < 32;
	}

	void Bullet::Delete()
	{
		// delete the bullet
	}

	//void Bullet::OnHit(const Enemy& enemy)
	//{
	//	m_speed *= 0.9;
	//	m_speed -= 15;
	//	m_speed = std::max({0, 0}, m_speed);
	//	m_penetration -= 1;
	//	if (!m_penetration)
	//	{
	//		//Delete();
	//	}
	//}


};