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
		sprite->SetFrame(frame);
		vec2 midPos = { pos.x - sprite->GetWidth() / 2, pos.y - sprite->GetHeight() / 2 };
		sprite->DrawScaled(static_cast<int>(midPos.x), static_cast<int>(midPos.y), sprite->GetWidth() * scale, sprite->GetHeight() * scale, _screen, false); // draw centered around point
	}
};