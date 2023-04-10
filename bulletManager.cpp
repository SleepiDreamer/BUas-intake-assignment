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
        Bullet* element = pool[n_active];
        element->setActive(true);
        element->setPos(_pos);
        element->setVel(_vel);
        element->setSize(_scale);
        element->setDamage(_damage);
        n_active++;
    }

    void BulletManager::disable(int _id)
    {
        int current = 0;
        int i = 0;
        for (Bullet* element : pool)
        {
            if (element->getId() == _id)
            {
                current = i;
            }
            i++;
        }
        pool[current]->reset();
        pool[current]->setPos({ -100, -100 });
        std::swap(pool[current], pool[--n_active]);
    }
    
    void BulletManager::render(Surface* _screen)
    {
        for (int i = 0; i < n_active; i++)
        {
            pool[i]->render(_screen);
            _screen->Circle(pool[i]->getPos(), 3, 0x00ff00);
        }
    }

    void BulletManager::update(vec2 _playerPos, float _dt)
    {
        for (int i = 0; i < n_active; i++)
        {
            Bullet* elm = (pool[i]);
            if (elm->update(_dt)) // calls update and checks if bullet should be disabled (return type bool)
            {
	            disable(elm->getId());
            }
            /*if (elm->getPos().x < 0 || elm->getPos().x > ScreenWidth || elm->getPos().y < 0 || elm->getPos().y > ScreenHeight)
            {
                disable(elm->getId());
            }*/
        }
    }
}
