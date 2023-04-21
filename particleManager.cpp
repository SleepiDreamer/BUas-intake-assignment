#include "particleManager.h"
#include "bullet.h"
#include "enemy.h"

namespace Tmpl8
{
    void ParticleManager::init(Surface* _screen, const std::shared_ptr<Sprite>& _sprite)
    {
        for (int i = 0; i < size; i++)
        {
            pool.emplace_back(new Particle({ 0, 0 }, _screen, _sprite, i));
        }
    }

    void ParticleManager::enable(vec2 _pos, vec2 _vel, float _scale, float _lifetime)
    {
        Particle* element = pool[n_active++];
        element->setActive(true);
        element->setPos(_pos);
        element->setVel(_vel);
        element->setSize(_scale);
        element->setLifetime(_lifetime);
        element->setScale(_scale);
    }

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

    void ParticleManager::render(const Surface* _screen) const
    {
        for (int i = 0; i < n_active; i++)
        {
            pool[i]->render(_screen);
        }
    }

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

    void ParticleManager::clear()
    {
        for (int i = 0; i < n_active; i++)
        {
            Particle* bullet = pool[i];
            bullet->setActive(false);
        }
        n_active = 0;
    }

    void ParticleManager::enemyDied(vec2 _pos)
    {
        for (int i = 0; i < 5; i++)
        {
	        const float angle = randfloat(0, 2 * PI);
			vec2 vel = { cos(angle), sin(angle) };
            vel *= randfloat(150.0f, 500.0f);
            enable(_pos, vel, randfloat(0.75f, 1.25f), randfloat(1.5f, 2.5f));
        }
    }
}
