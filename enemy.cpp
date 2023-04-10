#include "enemy.h"

#include "player.h"
#include "util.h"

namespace Tmpl8
{
	void Enemy::update(float _dt)
	{
		pos += vel * _dt;
	}

	void Enemy::render(Surface* _screen)
	{
		sprite->SetFrame(frame);
		vec2 midPos = { pos.x - sprite->GetWidth() / 2, pos.y - sprite->GetHeight() / 2 };
		sprite->DrawScaled(this->getTopLeft().x, this->getTopLeft().y, sprite->GetWidth() * scale, sprite->GetHeight() * scale, _screen, false); // draw centered around point
	}
};