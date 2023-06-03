#include "powerup.h"

#include "player.h"
#include "util.h"

namespace Tmpl8
{
    int Powerup::bulletCollisionCheck(const Bullet* _bullet)
    {
        if (_bullet->getPos().x > pos.x - size.x / 2 && _bullet->getPos().x < pos.x + size.x / 2 + _bullet->getHitboxSize() && _bullet->getPos().y > pos.y - size.y / 2 && _bullet->getPos().y < pos.y + size.y / 2 + _bullet->getHitboxSize())
    	{
            hp -= _bullet->getDamage();
            if (hp <= 0)
            {
                active = false;
                return 2;
            }
            return 1;
        }
        return 0;
    }

    void Powerup::spawn()
    {
        active = true;
		hp = maxHp;
		lifetime = 6.0f;
		pos = { randfloat(160, ScreenWidth - 160), randfloat (160, ScreenHeight - 160) };
        scale = 1.0f;
    }

    void Powerup::consume()
    {
        active = false;
        hp = maxHp;
        lifetime = 0.0f;
    }

    void Powerup::update(float _dt)
    {
        if (whiteFlashTimer > 0.0f) whiteFlashTimer -= _dt * 12;
    }

    void Powerup::render(Surface* _screen)
	{
        if (active) {
            scale = 1.0f + whiteFlashTimer / 6.0f; // scale up when hit
            screen->BarShadow({ getTopLeft().x - 5, getTopLeft().y + 5 }, { getBottomRight().x - 5, getBottomRight().y + 5 }, 10.0f, 0.2f);
            const float barWidth = lifetime * 10;
            screen->BarShadow({ pos.x - barWidth / 2 * scale - 5, getBottomRight().y * scale + 15 }, { pos.x + barWidth / 2 - 5, getBottomRight().y + 25 }, 10.0f, 0.2f);
            screen->Bar(pos.x - barWidth / 2, getBottomRight().y + 10, pos.x + barWidth / 2, getBottomRight().y + 20, 0xffffff); // lifetime
            screen->Bar(pos - size.x / 2 * scale, pos + size.x / 2 * scale, 0xffb500); // body
            screen->BoxThick(pos - size.x / 2 * scale, pos + size.x / 2 * scale, 5, 0xffd700); // outline
            if (whiteFlashTimer > 0.0f) screen->Bar(pos - size.x / 2 * scale - 5, pos + size.x / 2 * scale + 5, 0xffffff, whiteFlashTimer); // white flash when hit
            screen->PrintScaled("?", static_cast<int>(pos.x - 15), static_cast<int>(pos.y - 12), 5, 5, 0xffffff); // question mark
        }
	}
};