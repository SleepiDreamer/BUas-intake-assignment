#include "particleManager.h"
#include "bullet.h"
#include "enemy.h"

namespace Tmpl8
{
    // initialize the pool of bullet objects
    void ParticleManager::init(Surface* _screen, const std::shared_ptr<Sprite>& _sprite)
    {
        for (int i = 0; i < size; i++)
        {
            pool.emplace_back(new Particle({ 0, 0 }, _screen, _sprite, i));
        }
    }

    // enable a bullet in the pool at a given position, velocity, scale and damage
    void ParticleManager::enable(vec2 _pos, vec2 _vel, float _scale, float _lifetime, int _type)
    {
        Particle* element = pool[n_active++];
        element->setActive(true);
        element->setPos(_pos);
        element->setVel(_vel);
        element->setSize(_scale);
        element->setLifetime(_lifetime);
        element->setScale(_scale);
        element->setType(_type);
    }

    // disable a specified bullet in the pool
    void ParticleManager::disable(int _id)
    {
        int current = 0;
        int i = 0;
        for (const Particle* bullet : pool)
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

    // render all the active bullets in the pool
    void ParticleManager::render(const Surface* _screen) const
    {
        for (int i = 0; i < n_active; i++)
        {
            pool[i]->render(_screen);
        }
    }

    // update all the active bullets in the pool
    void ParticleManager::update(const float _dt)
    {
        for (int i = 0; i < n_active; i++)
        {
            Particle* elm = (pool[i]);
            elm->update(_dt);
            if (elm->getLifetime() <= 0.0f || elm->getPos().x < 0 || elm->getPos().x > ScreenWidth || elm->getPos().y < 0 || elm->getPos().y > ScreenHeight)
            {
                disable(elm->getId());
            }
        }
    }

    // set all the active bullets in the pool to inactive
    void ParticleManager::clear()
    {
        for (int i = 0; i < n_active; i++)
        {
            Particle* bullet = pool[i];
            bullet->setActive(false);
        }
        n_active = 0;
    }

    // spawns an "enemy splatter" when an enemy was killed by the player
    void ParticleManager::enemyDied(vec2 _pos)
    {
        for (int i = 0; i < 5; i++)
        {
            const vec2 pos = { randfloat(_pos.x - 10, _pos.x + 10), randfloat(_pos.y - 10, _pos.y + 10) };
	        const float angle = randfloat(0, 2 * PI);
			vec2 vel = { cos(angle), sin(angle) };
            vel *= randfloat(150.0f, 500.0f);
            enable(pos, vel, randfloat(0.75f, 1.25f), randfloat(0.75f, 1.25f), 0);
        }
    }

    // spawns a powerup explosion when a powerup was consumed by the player
    void ParticleManager::powerupConsumed(vec2 _pos)
    {
        for (int i = 0; i < 35; i++)
        {
            const vec2 pos = { randfloat(_pos.x - 50, _pos.x + 50), randfloat(_pos.y - 50, _pos.y + 50) };
            const float angle = randfloat(0, 2 * PI);
            vec2 vel = { cos(angle), sin(angle) };
            vel *= randfloat(750.0f, 2500.0f);
            enable(pos, vel, randfloat(1.0f, 2.5f), randfloat(0.75f, 1.5f), 1);
        }
    }

    // spawns a healing effect when the player was healed by a powerup
    void ParticleManager::playerHealed(vec2 _pos)
    {
        for (int i = 0; i < 5; i++)
        {
            const vec2 pos = { randfloat(_pos.x - 30, _pos.x + 30), randfloat(_pos.y - 30, _pos.y + 30) };
            enable(pos, 0, randfloat(1.25f, 2.0f), randfloat(0.75f, 1.25f), 2);
        }
    }
}
