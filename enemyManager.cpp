#include "enemyManager.h"

#include <string>
#include <iostream>
#include <cstring>

#include "enemy.h"
#include "util.h"

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
        element->setHP(50);
        n_active++;
    }

    void EnemyManager::disable(int _id)
    {
        int current = 0;
        int i = 0;
        for (Enemy* enemy : pool)
        {
            if (enemy->getId() == _id)
            {
                current = i;
                break;
            }
            i++;
        }
        pool[current]->setActive(false);
        std::swap(pool[current], pool[--n_active]);
    }

    void EnemyManager::render(Surface* _screen)
    {
        for (int i = 0; i < n_active; i++)
        {
            Enemy* enemy = pool[i];
            enemy->render(_screen);
            //_screen->Circle(pool[i]->getPos(), pool[i]->getHitboxSize(), 0x00ff00); // DEBUG
            //_screen->PrintScaled(intToChar(enemy->getId()), enemy->getPos().x, enemy->getPos().y - 20, 3, 3, 0x00ff00);
        }
    }

    void EnemyManager::update(vec2 _playerPos, float _dt)
    {
        for (int i = 0; i < n_active; i++)
        {
            Enemy* enemy = (pool[i]);
            enemy->update(_dt);
            enemy->PointTowards(_playerPos);
            enemy->MoveTowards(_playerPos, 100.0f);
        }
    }

    void EnemyManager::clear()
    {
        for (int i = 0; i < n_active; i++)
        {
            Enemy* enemy = pool[i];
            std::cout << enemy->getId() << std::endl;
            enemy->setActive(false);
            //enemy->setPos({ 0, 0 });
        }
        n_active = 0;
    }

    bool EnemyManager::bulletCollisionCheck(Bullet* _bullet)
    {
        // go through all enemies
        for (int i = 0; i < n_active; i++)
        {
            Enemy* enemy = pool[i];
            if (distanceBetween(_bullet->getPos(), enemy->getPos()) < enemy->getHitboxSize())
            {
                enemy->setHP(enemy->getHP() - _bullet->getDamage());
                std::cout << "hit enemy " << enemy->getId() << std::endl;
                if (enemy->getHP() <= 0)
                {
                    disable(enemy->getId());
                }
                return true;
            }
        }
        return false;
    }

    bool EnemyManager::playerCollisionCheck(Player* _player)
    {
        for (int i = 0; i < n_active; i++)
        {
            Enemy* enemy = pool[i];
            if (distanceBetween(_player->getPos(), enemy->getPos()) < enemy->getHitboxSize() + _player->getSize().x / 2)
            {
                std::cout << "player hit!" << std::endl;
                return true;
            }
        }
        return false;
    }
}
