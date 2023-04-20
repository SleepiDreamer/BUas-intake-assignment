#include "bullet.h"

#include "player.h"
#include "util.h"

namespace Tmpl8
{
	void Bullet::update(float _dt)
	{
		pos += vel * _dt;
	}

	void Bullet::render(Surface* _screen) const
	{
		//sprite->DrawScaled(pos.x, pos.y, size.x * scale, size.y * scale, _screen, false); // draw sprite centered around point
		_screen->CircleShadow({ pos.x - 5, pos.y + 5 }, 6, 0.25f);
		_screen->CircleFull(pos, 0, 3, 0xff4400);
	}
};