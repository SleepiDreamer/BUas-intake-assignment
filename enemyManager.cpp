#include "enemyManager.h"
#include "enemy.h"

namespace Tmpl8
{
    void EnemyManager::init(Surface* _screen, std::shared_ptr<Sprite> _sprite)
    {
        for (int i = 0; i < size; i++)
        {
            pool.emplace_back(new Enemy({ 0, 0 }, _screen, _sprite, i));
        }
        std::cout << "done!" << std::endl;
    }

    void EnemyManager::enable(vec2 _pos, vec2 _vel, float _scale, float _damage)
    {
        Enemy* element = pool[n_active];
        element->setActive(true);
        element->setPos(_pos);
        element->setVel(_vel);
        element->setSize(_scale);
        element->setDamage(_damage);
        n_active++;
    }

    void EnemyManager::disable(int _id)
    {
        int current = 0;
        int i = 0;
        for (Enemy* element : pool)
        {
            if (element->getId() == _id)
            {
                current = i;
            }
            i++;
        }
        if (current != n_active - 1) std::swap(pool[current], pool[n_active - 1]);
        pool[current]->setActive(false);
        n_active--;
    }

    void EnemyManager::render(Surface* _screen)
    {
        for (int i = 0; i < n_active; i++)
        {
            pool[i]->render(_screen);
            _screen->Circle(pool[i]->getPos(), pool[i]->getHitboxSize(), 0x00ff00);
        }
    }

    void EnemyManager::update(vec2 _playerPos, float _dt)
    {
        for (int i = 0; i < n_active; i++)
        {
            Enemy* elm = (pool[i]);
            elm->update(_dt);
            elm->PointTowards(_playerPos);
            elm->MoveTowards(_playerPos, 100.0f);
        }
    }

    bool EnemyManager::collisionCheck(Bullet* _bullet)
    {
        // go through all enemies
        for (Enemy* enemy : pool)
        {
            if (enemy->getActive() && distanceBetween(_bullet->getPos(), enemy->getPos()) < enemy->getHitboxSize())
            {
                enemy->setHP(enemy->getHP() - _bullet->getDamage());
                std::cout << "hit!" << std::endl;
                if (enemy->getHP() <= 0)
                {
                    disable(enemy->getId());
                }
                return true;
            }
        }
        return false;
    }
}
