#include "enemy.h"

namespace Tmpl8
{

	void Enemy::SetPos(const vec2& newPos)
	{
		m_pos = newPos;
	}
	const vec2& Enemy::GetPos() const
	{
		return m_pos;
	}

	void Enemy::SetDir(const vec2& newDir)
	{
		m_dir = newDir;
	}
	const vec2& Enemy::GetDir() const
	{
		return m_dir;
	}

	void Enemy::SetSpeed(const vec2& newSpeed)
	{
		m_speed = newSpeed;
	}
	const vec2& Enemy::GetSpeed() const
	{
		return m_speed;
	}

	void Enemy::PointTowards(float px, float py)
	{
		m_dir = atan2(py - m_pos.y, px - m_pos.x) + PI;
		//m_frame = (unsigned int)(m_dir / (2 * PI) * m_sprite->Frames()) % m_sprite->Frames();
	}

	void Enemy::MoveTowards(const float px, const float py, const float dist)
	{
		const float dir2p = atan2(py - m_pos.y, px - m_pos.x);
		m_pos.x += cos(dir2p) * dist;
		m_pos.y += sin(dir2p) * dist;
	}

	void Enemy::Render(Surface* gameScreen)
	{
		m_sprite->SetFrame(m_frame);
		m_sprite->Draw(gameScreen, m_pos.x - (m_width / 2), m_pos.y - (m_height / 2));
	}
}