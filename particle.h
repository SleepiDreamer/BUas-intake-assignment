#pragma once
#include <vector>

#include "surface.h"
#include "template.h"
#include "entity.h"

namespace Tmpl8 {
	class Particle : public Entity
	{
	private:
		bool active;
		float scale = 1.0f; // factor to multiply the sprite size with
		int id;
		float radius = 5.0f;
		float lifetime;
		std::vector<int> pastels = { 0xB2A4FF, 0xFFB4B4, 0xFFDEB4, 0xFDF7C3, 0xF7C8E0, 0xDFFFD8, 0xB4E4FF, 0x95BDFF };
	public:
		void setActive(const bool _active) { active = _active; }
		bool getActive() const { return active; }
		void setId(const int _id) { id = _id; }
		int getId() const { return id; }
		float getLifetime() const { return lifetime; }
		void setLifetime(const float _lifetime) { lifetime = _lifetime; }

		Particle(vec2 _pos, Surface* _screen, std::shared_ptr<Sprite> _sprite, const int _id) :
			Entity(_pos, _screen, std::move(_sprite))
		{
			id = _id;
			vel = { 0, 0 };
			if (sprite != nullptr) size = { static_cast<float>(sprite->GetWidth()) * scale, static_cast<float>(sprite->GetHeight()) * scale }; // size of the sprite in pixels
			else size = { 5, 5 };
			lifetime = 0.0f;
			active = false;
		}

		void update(float _dt);

		void render(const Surface* _screen) const;
	};
}