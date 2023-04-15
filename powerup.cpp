#include "powerup.h"

#include "player.h"
#include "util.h"

namespace Tmpl8
{
    int Powerup::bulletCollisionCheck(Bullet* _bullet)
    {
        if (distanceBetween(_bullet->getPos(), pos) < hitboxSize * scale + _bullet->getSize().x / 2)
        {
            hp -= _bullet->getDamage();
            std::cout << "hit powerup" << std::endl;
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

	void Powerup::render(Surface* _screen)
	{
        if (active) {
            screen->CircleFull(pos, 0, static_cast<int>(scale * size.x / 2.0f), 0xffa071);
            screen->CircleFull(pos, scale * 25, static_cast<int>(scale * size.x / 2.0f), 0xfedd9e);
        }
	}
};