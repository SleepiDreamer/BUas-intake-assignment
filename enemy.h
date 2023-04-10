#pragma once
#include "surface.h"
#include "entity.h"

namespace Tmpl8 {
	class Enemy : public Entity
	{
	private:
		float hp;
		int id;
		bool active;

		float hitboxSize = 20.0f;
		float speedMax = 450.0f;
		float speedAccel = 2000.0f;
		float lastShot = 0;
		float shotDelay = 1.0f;
		float scale = 1.0f;

	public:
		void setHP(float _hp) { hp = _hp; }
		float getHP() const { return hp; }
		void setActive(bool _active) { active = _active; }
		bool getActive() const { return active; }
		void setId(int _id) { id = _id; }
		int getId() const { return id; }
		float getHitboxSize() const { return hitboxSize; }

		Enemy(vec2 _pos, Surface* _screen, std::shared_ptr<Sprite> _sprite, int _id) :
			Entity(_pos, _screen, std::move(_sprite))
		{
			id = _id;
			active = false;
			vel = { 0, 0 };
			size = { static_cast<float>(sprite->GetWidth()) * scale, static_cast<float>(sprite->GetHeight()) * scale };
			damage = 0;
			hp = 50;
		}

		void update(float _dt);

		void render(Surface* _screen);
	};
}