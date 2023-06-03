#include "entity.h"

namespace Tmpl8
{
	void Entity::PointTowards(vec2 _pos)
	{
		const float dx = _pos.x - pos.x;
		const float dy = _pos.y - pos.y;
		const float angle = static_cast<float>(atan2(dy, dx));
		dir = { static_cast<float>(cos(angle)), static_cast<float>(sin(angle)) };
	}

	void Entity::MoveTowards(vec2 _pos, float _speed)
	{
		const float dx = _pos.x - pos.x;
		const float dy = _pos.y - pos.y;
		const float angle = static_cast<float>(atan2(dy, dx));
		vel = { static_cast<float>(cos(angle)), static_cast<float>(sin(angle)) };
		vel *= _speed;
	}
}