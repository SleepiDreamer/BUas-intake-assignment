#include "bullet.h"

#include "player.h"
#include "util.h"

namespace Tmpl8
{
	void Bullet::update(float _dt)
	{
		pos += vel * _dt;
	}

	void Bullet::render(Surface* _screen)
	{
		//sprite->DrawScaled(pos.x, pos.y, size.x * scale, size.y * scale, _screen, false); // draw centered around point
		_screen->CircleFull(pos, 0, 3, 0xff4400);
	}
};