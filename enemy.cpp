#include "enemy.h"

#include "surface.h"
#include "player.h"
#include "util.h"

namespace Tmpl8
{
	void Enemy::update(float _dt)
	{
		pos += vel * _dt;
		if (whiteFlashTimer > 0.0f) { whiteFlashTimer -= _dt * 12; }
	}

	void Enemy::render(Surface* _screen)
	{
		//sprite->DrawScaled(this->getTopLeft().x, this->getTopLeft().y, sprite->GetWidth() * scale, sprite->GetHeight() * scale, _screen, false); // draw centered around point
		srand(id);
		int color = pastels[id % static_cast<int>(pastels.size())];
		if (whiteFlashTimer > 0.0f) { color = static_cast<int>(AlphaBlend(color, 0xffffff, whiteFlashTimer)); }
		screen->CircleShadow({ pos.x - 5, pos.y + 5 }, 27, 0.7f);
		screen->CircleFull(pos, 0, 20, color);
	}
};