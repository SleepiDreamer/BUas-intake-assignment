#include "powerup.h"

#include "player.h"
#include "util.h"

namespace Tmpl8
{
    int Powerup::bulletCollisionCheck(Bullet* _bullet)
    {
        //if (distanceBetween(_bullet->getPos(), pos) < hitboxSize * scale + _bullet->getSize().x / 2)
        // if inside hitbox
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

	void Powerup::render(Surface* _screen)
	{
        if (active) {
            /*screen->CircleFull(pos, 0, static_cast<int>(scale * size.x / 2.0f), 0xffa071);
            screen->CircleFull(pos, scale * 25, static_cast<int>(scale * size.x / 2.0f), 0xfedd9e);*/
            screen->BarShadow({ getTopLeft().x - 5, getTopLeft().y + 5 }, { getBottomRight().x - 5, getBottomRight().y + 5 }, 10.0f, 0.2f);
        	const float barWidth = lifetime * 10;
            screen->BarShadow({ pos.x - barWidth / 2 - 5, getBottomRight().y + 15 }, { pos.x + barWidth / 2 - 5, getBottomRight().y + 25 }, 10.0f, 0.2f);
        	screen->Bar(pos.x - barWidth / 2, getBottomRight().y + 10, pos.x + barWidth / 2, getBottomRight().y + 20, 0xffffff); // lifetime
        	screen->Bar(pos * scale- size.x / 2, pos * scale+ size.x / 2, 0xffa071); // body
            screen->BoxThick(pos - size.x / 2, pos + size.x / 2, 5, 0xfedd9e); // outline
            screen->PrintScaled("?", pos.x - 15, pos.y - 12, 5, 5, 0xffffff); // question mark
            
            
        }
	}
};