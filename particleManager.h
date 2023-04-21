#pragma once
#include "surface.h"
#include "bullet.h"
#include "util.h"
#include <vector>
#include <iostream>

#include "particle.h"

namespace Tmpl8 {
	class ParticleManager
	{
	private:
		std::vector<Particle*> pool;
		int n_active = 0;
		int size;
		// TYPES:
		// 0: enemy splatter
		// 1: player heal
	public:
		void init(Surface* _screen, const std::shared_ptr<Sprite>& _sprite);
		void enable(vec2 _pos, vec2 _vel, float _scale, float _lifetime, int _type);
		void disable(int id);
		int getActiveBullets() const { return n_active; }
		int getSize() const { return size; }


		std::vector<Particle*> getPool() { return pool; }

		void render(const Surface* _screen) const;
		void update(float _dt);
		void clear();

		void enemyDied(vec2 _pos);
		void playerHealed(vec2 _pos);

		ParticleManager(int _size)
		{
			pool.reserve(_size);
			size = _size;
		}
	};
}
