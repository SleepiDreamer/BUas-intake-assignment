#include "bulletManager.h"
#include "bullet.h"
#include "enemy.h"

namespace Tmpl8
{
    void BulletManager::init(Surface* _screen, std::shared_ptr<Sprite> _sprite)
    {
        for (int i = 0; i < size; i++)
        {
            pool.emplace_back(new Bullet({ 0, 0 }, _screen, _sprite, i));
        }
        std::cout << "done!" << std::endl;
    }

    void BulletManager::enable(vec2 _pos, vec2 _vel, float _scale, float _damage)
    {
        Bullet* element = pool[n_active++];
        element->setActive(true);
        element->setPos(_pos);
        element->setVel(_vel);
        element->setSize(_scale);
        element->setDamage(_damage);
    }

    void BulletManager::disable(int _id)
    {
        int current = 0;
        int i = 0;
        for (Bullet* bullet : pool)
        {
            if (bullet->getId() == _id)
            {
                current = i;
                break;
            }
            i++;
        }
        if (current != n_active - 1) std::swap(pool[current], pool[n_active - 1]);
        pool[n_active - 1]->setActive(false);
    	n_active--;
    }
    
    void BulletManager::render(Surface* _screen)
    {
        for (int i = 0; i < n_active; i++)
        {
            pool[i]->render(_screen);
        }
    }

    void BulletManager::update(vec2 _playerPos, float _dt)
    {
        for (int i = 0; i < n_active; i++)
        {
            Bullet* elm = (pool[i]);
            elm->update(_dt);
            if (elm->getPos().x < 0 || elm->getPos().x > ScreenWidth || elm->getPos().y < 0 || elm->getPos().y > ScreenHeight)
            {
                disable(elm->getId());
            }
        }
    }

    // TODO: implement clear() function
}
