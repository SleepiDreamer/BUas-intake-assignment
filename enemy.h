#pragma once
#include <vector>

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
		float scale = 1.0f;
		float whiteFlashTimer = 0.0f;
		std::vector<int> pastels = { 0xB2A4FF, 0xFFB4B4, 0xFFDEB4, 0xFDF7C3, 0xF7C8E0, 0xDFFFD8, 0xB4E4FF, 0x95BDFF };

	public:
		void setHP(float _hp) { hp = _hp; }
		float getHP() const { return hp; }
		void setActive(bool _active) { active = _active; }
		bool getActive() const { return active; }
		void setId(int _id) { id = _id; }
		int getId() const { return id; }
		float getHitboxSize() const { return hitboxSize * scale; }
		void setWhiteFlashTimer(float _whiteFlashTimer) { whiteFlashTimer = _whiteFlashTimer; }

		Enemy(vec2 _pos, Surface* _screen, std::shared_ptr<Sprite> _sprite, int _id) :
			Entity(_pos, _screen, std::move(_sprite))
		{
			id = _id;
			active = false;
			vel = { 0, 0 };
			if (sprite != nullptr) size = { static_cast<float>(sprite->GetWidth()) * scale, static_cast<float>(sprite->GetHeight()) * scale };
			else size = { 50, 50 };
			damage = 0;
			hp = 60;
		}

		void update(float _dt);

		void render(Surface* _screen);
	};
}