#pragma once
#include "bullet.h"
#include "surface.h"
#include "entity.h"

namespace Tmpl8 {
	class Powerup : public Entity
	{
	private:
		float hp;
		float maxHp = 250.0f;
		float lifetime = 5.0f;
		float hitboxSize = 20.0f;
		float scale = 1.0f;
		bool active = false;
		float whiteFlashTimer = 0.0f;
	public:
		void setHP(float _hp) { hp = _hp; }
		float getHP() const { return hp; }
		void setLifetime(float _lifetime) { lifetime = _lifetime; }
		float getLifetime() const { return lifetime; }
		float getHitboxSize() const { return hitboxSize; }
		void setActive(bool _active) { active = _active; }
		bool getActive() const { return active; }
		void setScale(float _scale) { scale = _scale; }
		float getScale() const { return scale; }
		float getMaxHp() const { return maxHp; }
		float getWhiteFlashTimer() const { return whiteFlashTimer; }
		void setWhiteFlashTimer(float _whiteFlashTimer) { whiteFlashTimer = _whiteFlashTimer; }

		Powerup(vec2 _pos, Surface* _screen, std::shared_ptr<Sprite> _sprite) :
			Entity(_pos, _screen, std::move(_sprite))
		{
			vel = { 0, 0 };
			if (sprite != nullptr) size = { static_cast<float>(sprite->GetWidth()) * scale, static_cast<float>(sprite->GetHeight()) * scale };
			else size = { 60, 60 };
			damage = 0;
			hp = maxHp;
		}

		int bulletCollisionCheck(Bullet* _bullet); // 0 = no collision, 1 = collision, 2 = collision and destroy

		void spawn();

		void consume();

		void update(float _dt);

		void render(Surface* _screen);
	};
}