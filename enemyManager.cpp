#include "enemyManager.h"

#include <string>
#include <iostream>
#include <cstring>

#include "enemy.h"
#include "util.h"

namespace Tmpl8
{
    void EnemyManager::init(Surface* _screen, const std::shared_ptr<Sprite>& _sprite)
    {
        for (int i = 0; i < size; i++)
        {
            pool.emplace_back(new Enemy({ 0, 0 }, _screen, _sprite, i));
        }
    }

    void EnemyManager::enable(const vec2 _pos, const vec2 _vel, const float _scale, const float _damage)
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

    void EnemyManager::disable(const int _id)
    {
        int current = 0;
        int i = 0;
        for (const Enemy* enemy : pool)
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

    void EnemyManager::render(Surface* _screen) const
    {
        for (int i = 0; i < n_active; i++)
        {
            Enemy* enemy = pool[i];
            enemy->render(_screen);
            //_screen->Circle(pool[i]->getPos(), pool[i]->getHitboxSize(), 0x00ff00); // DEBUG
            //_screen->PrintScaled(intToChar(enemy->getId()), enemy->getPos().x, enemy->getPos().y - 20, 3, 3, 0x00ff00);
        }
    }

    void EnemyManager::update(const vec2 _playerPos, const float _dt) const
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
            enemy->setActive(false);
            //enemy->setPos({ 0, 0 });
        }
        n_active = 0;
    }

    int EnemyManager::bulletCollisionCheck(const Bullet* _bullet)
    {
        // go through all enemies
        for (int i = 0; i < n_active; i++)
        {
            Enemy* enemy = pool[i];
            if (distanceBetween(_bullet->getPos(), enemy->getPos()) < enemy->getHitboxSize() + _bullet->getHitboxSize())
            {
                enemy->setHP(enemy->getHP() - _bullet->getDamage());
                if (enemy->getHP() <= 0)
                {
                    disable(enemy->getId());
                    return 2;
                }
                return 1;
            }
        }
        return 0;
    }

    bool EnemyManager::playerCollisionCheck(const Player* _player) const
    {
        for (int i = 0; i < n_active; i++)
        {
            const Enemy* enemy = pool[i];
            if (distanceBetween(_player->getPos(), enemy->getPos()) < enemy->getHitboxSize() + _player->getSize().x / 2)
            {
                return true;
            }
        }
        return false;
    }
}
