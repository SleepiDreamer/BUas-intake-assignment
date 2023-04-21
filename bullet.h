#pragma once
#include "surface.h"
#include "template.h"
#include "entity.h"

namespace Tmpl8 {
	class Bullet : public Entity
	{
	private:
		bool active;
		float scale = 2.0f; // factor to multiply the sprite size with
		int id;
	public:
		void setActive(bool _active) { active = _active; }
		bool getActive() const { return active; }
		void setId(int _id) { id = _id; }
		int getId() const { return id; }
		float getHitboxSize() const { return size.x * scale; }

		Bullet(vec2 _pos, Surface* _screen, std::shared_ptr<Sprite> _sprite, const int _id) :
			Entity(_pos, _screen, std::move(_sprite))
		{
			id = _id;
			vel = { 0, 0 };
			if (sprite != nullptr) size = { static_cast<float>(sprite->GetWidth()) * scale, static_cast<float>(sprite->GetHeight()) * scale }; // size of the sprite in pixels
			else size = { 5, 5 };
			damage = 10.0f;
			active = false;
		}

		void update(float _dt);

		void render(const Surface* _screen) const;
	};
}