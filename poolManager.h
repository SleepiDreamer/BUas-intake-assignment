#pragma once
#include "surface.h"
#include "bullet.h"
#include "util.h"
#include <vector>
#include <iostream>

namespace Tmpl8 {
	template <typename T, int N>
	class PoolManager
	{
	private:
		std::vector<T*> pool;
		int n_active = 0;
	public:
		void init(Surface* _screen, std::shared_ptr<Sprite> _sprite);
		void enable(vec2 _pos, vec2 _vel, float _scale, float _damage);
		void disable(int id);

		std::vector<T*> getPool() { return pool; }

		void render(Surface* _screen);
		void update(vec2 _playerPos, float _dt);
		void collisionCheck(Bullet* _bullet, float _distance);

		PoolManager()
		{
			pool.reserve(N);
		}
	};
}