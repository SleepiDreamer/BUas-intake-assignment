#pragma once
#include "surface.h"
#include "bullet.h"
#include "util.h"
#include <vector>
#include <iostream>

namespace Tmpl8 {
	class BulletManager
	{
	private:
		std::vector<Bullet*> pool;
		int n_active = 0;
		int size;
	public:
		void init(Surface* _screen, const std::shared_ptr<Sprite>& _sprite);
		void enable(vec2 _pos, vec2 _vel, float _scale, float _damage);
		void disable(int id);
		int getActiveBullets() const { return n_active; }
		int getSize() const { return size;  }

		std::vector<Bullet*> getPool() { return pool; }

		void render(Surface* _screen) const;
		void update(vec2 _playerPos, float _dt);
		void clear();

		BulletManager(int _size)
		{
			pool.reserve(_size);
			size = _size;
		}
	};
}