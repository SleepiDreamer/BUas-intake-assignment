#include "particle.h"
#include <cmath>
#include <windows.h>
#include "util.h"

namespace Tmpl8
{
	void Particle::update(float _dt)
	{
		pos += vel * _dt;
		lifetime -= _dt;
		switch (type)
		{
		case 0: // enemy splatter
			{
				vel *= pow(0.00015f, _dt); // dampening
				break;
			}
		case 1: // player heal
			{
				vel.y -= 50.0f * _dt;
				break;
			}
		}
	}

	void Particle::render(const Surface* _screen) const
	{
		//sprite->DrawScaled(pos.x, pos.y, size.x * scale, size.y * scale, _screen, false); // draw sprite centered around point
		switch (type) {
		case 0:
			{
			const float alpha = min(1.0f, lifetime) * 0.9f;
				_screen->CircleShadow({ pos.x - 5, pos.y + 5 }, scale * radius * 1.3f, 0.25f * alpha * 2);
				const int color = pastels[id % static_cast<int>(pastels.size())];
				_screen->CircleFull(pos, 0, static_cast<int>(scale * radius * 1.3f), color, alpha * 2);
				break;
			}
		case 1:
			{
				const float alpha = min(1.0f, lifetime) * 0.85f;
				_screen->CircleShadow({ pos.x - 5, pos.y + 5 }, scale * radius * 1.3f, 0.25f * alpha);
				const int color = 0xe80000;
				_screen->CircleFull(pos, 0, static_cast<int>(scale * radius * 1.3f), color, alpha);
				break;
			}
		}
			
	}
};