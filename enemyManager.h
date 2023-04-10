#pragma once
#pragma once
#include "surface.h"
#include "enemy.h"
#include "util.h"
#include <vector>
#include <iostream>

#include "bullet.h"

namespace Tmpl8 {
	class EnemyManager
	{
	private:
		std::vector<Enemy*> pool;
		int n_active = 0;
		int size;
	public:
		void init(Surface* _screen, std::shared_ptr<Sprite> _sprite);
		void enable(vec2 _pos, vec2 _vel, float _scale, float _damage);
		void disable(int id);

		std::vector<Enemy*> getPool() { return pool; }

		void render(Surface* _screen);
		void update(vec2 _playerPos, float _dt);
		bool collisionCheck(Bullet* _bullet);

		EnemyManager(int _size)
		{
			pool.reserve(_size);
			size = _size;
		}
	};
}
