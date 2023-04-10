#pragma once
#include "surface.h"
#include "template.h"
#include "entity.h"
#include <utility>
#include <vector>

namespace Tmpl8 {
	class Bullet : public Entity
	{
	private:
		int bounces = 0;
		float gravity = 800.0f;
		float bounciness = 0.5f;
		bool active;
		float scale = 1.0f; // factor to multiply the sprite size with
		int id;
	public:
		vec2 getTopLeft() override { return { pos.x - (size.x * scale / 2), pos.y - (size.y * scale / 2) }; }
		vec2 getBottomRight() override { return { pos.x + (size.x * scale / 2), pos.y + (size.y * scale / 2) }; }
		void setActive(bool _active) { active = _active; }
		bool getActive() const { return active; }
		void reset();
		void setId(int _id) { id = _id; }
		int getId() const { return id; }

		Bullet(vec2 _pos, Surface* _screen, std::shared_ptr<Sprite> _sprite, int _id) :
			Entity(_pos, _screen, std::move(_sprite))
		{
			id = _id;
			vel = { 0, 0 };
			size = { static_cast<float>(sprite->GetWidth()) * scale, static_cast<float>(sprite->GetHeight()) * scale }; // size of the sprite in pixels
			damage = 10.0f;
			active = false;
		}

		bool update(float _dt);
		
		void render(Surface* _screen);
	};
}