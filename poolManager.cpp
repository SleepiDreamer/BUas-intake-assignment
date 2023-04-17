#include "poolManager.h"

#include "bullet.h"
#include "enemy.h"

namespace Tmpl8
{
    template <typename T, int N>
    void PoolManager<T, N>::init(Surface* _screen, std::shared_ptr<Sprite> _sprite)
    {
        for (int i = 0; i < N; i++)
        {
            pool.emplace_back(new T({ 0, 0 }, _screen, _sprite, i));
        }
    }

    template <typename T, int N>
    void PoolManager<T, N>::enable(vec2 _pos, vec2 _vel, float _scale, float _damage)
    {
        T* element = pool[n_active];
        element->setActive(true);
        element->setPos(_pos);
        element->setVel(_vel);
        element->setSize(_scale);
        element->setDamage(_damage);
        n_active++;
    }

    template <typename T, int N>
    void PoolManager<T, N>::disable(int _id)
    {
        int current = 0;
        int i = 0;
        for (T* element : pool)
        {
            if (element->getId() == _id)
            {
                current = i;
            }
            i++;
        }
        std::swap(pool[current], pool[n_active - 1]);
        n_active--;
    }

    template <typename T, int N>
    void PoolManager<T, N>::render(Surface* _screen)
    {
        for (int i = 0; i < n_active; i++)
        {
            pool[i]->render(_screen);
        }
    }

    template <typename T, int N>
    void PoolManager<T, N>::update(vec2 _playerPos, float _dt)
    {
        for (int i = 0; i < n_active; i++)
        {
            //if type of T is Bullet
            if (typeid(T) == typeid(Bullet)) // --- BULLET ---
            {
                T* elm = (pool[i]);
                elm->update(_dt);
                if (elm->getPos().x < 0 || elm->getPos().x > ScreenWidth || elm->getPos().y < 0 || elm->getPos().y > ScreenHeight)
                {
                    disable(elm->getId());
                }
			}
            else if (typeid(T) == typeid(Enemy)) // --- ENEMY ---
            {
                T* elm = (pool[i]);
                elm->update(_dt);
                elm->PointTowards(_playerPos);
                elm->MoveTowards(_playerPos, 100.0f);
            }
        		
        }
    }

    //template <typename T, int N>
    //void PoolManager<T, N>::collisionCheck(Bullet* _bullet, float _distance)
    //{
    //    // go through all enemies
    //    for (T* enemy : pool)
    //    {
    //        if (distanceBetween(_bullet->getPos(), enemy->getPos()) < _distance)
    //        {
    //            enemy->setHP(enemy->getHP() - _bullet->getDamage());
    //            if (enemy->getHP() <= 0)
    //            {
    //                disable(enemy->getId());
    //            }
    //        }
    //    }
    //}

    template class PoolManager<Bullet, 300>;
    template class PoolManager<Enemy, 200>;

}
