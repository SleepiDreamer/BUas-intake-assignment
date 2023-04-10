#include "bullet.h"

#include "player.h"
#include "util.h"

namespace Tmpl8
{
	void Bullet::reset()
	{
		active = false;
		pos = { 0, 0 };
		vel = { 0, 0 };
		scale = 1;
		bounces = 0;
	}

	bool Bullet::update(float _dt) // bool is for whether or not to destroy the bullet
	{
		vel.y += gravity * _dt;
		pos += vel * _dt;
		if (this->getBottomRight().y > ScreenHeight && vel.y > 0 && bounces < 3) // if bottom of sprite is past the bottom of the screen, bounce up. second condition is for extra redundancy
		{
			vel.y *= -bounciness;
			pos.y = ScreenHeight - size.y * scale / 2;
			bounces++;
		}
		return (pos.y > ScreenHeight + 200);
	}

	void Bullet::render(Surface* _screen)
	{
		sprite->SetFrame(frame);
		vec2 midPos = { pos.x, pos.y };
		sprite->DrawScaled(static_cast<int>(midPos.x), static_cast<int>(midPos.y), sprite->GetWidth() * scale, sprite->GetHeight() * scale, _screen, false); // draw centered around point
	}
};