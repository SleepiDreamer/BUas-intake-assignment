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
		//sprite->DrawScaled(this->getTopLeft().x, this->getTopLeft().y, sprite->GetWidth() * scale, sprite->GetHeight() * scale, _screen, false); // draw centered around point
		srand(id);
		int color = pastels[id % static_cast<int>(pastels.size())];
		screen->CircleShadow({ pos.x - 5, pos.y + 5 }, 27, 0.7f);
		screen->CircleFull(pos, 0, 20, color);
	}
};