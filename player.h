#pragma once
#include <iostream>

#include "surface.h"
#include "template.h"
#include "entity.h"


namespace Tmpl8 {
	class Player : public Entity
	{
	private:
		int weapon = 0;
		float maxHp = 3.0f;
		float hp = maxHp;
		float invincibility = 0.0f;
		float velMax = 450.0f;
		float acceleration = 1500.0f;
		float lastShot = 0.0f;
		float shotDelay = 0.05f;
		float scale = 1.0f; // factor to multiply the sprite size with
		bool isRunning = false;

	public:
		vec2 getTopLeft() override { return { pos.x - (size.x * scale / 2), pos.y - (size.y * scale / 2) }; }
		vec2 getBottomRight() override { return { pos.x + (size.x * scale / 2), pos.y + (size.y * scale / 2) }; }
		void setWeapon(int _weapon) { weapon = _weapon; }
		int getWeapon() const { return weapon; }
		void setHp(float _hp) { hp = _hp; }
		float getHp() const { return hp; }
		float getMaxHp() const { return maxHp; }
		bool canShoot() const { return (lastShot > shotDelay); }
		void resetShotTimer() { lastShot = 0; }
		void setInvincibility(float _invincibility) { invincibility	= _invincibility; }
		float getInvincibility() const { return invincibility; }
		bool playerDamaged();
		void setShotDelay(float _shotDelay) { shotDelay = _shotDelay; }
		float getShotDelay() const { return shotDelay; }

		Player(vec2 _pos, Surface* _screen, std::unique_ptr<Sprite> _sprite) :
			Entity(_pos, _screen, std::move(_sprite))
		{
			vel = { -300, 100 };
			if (sprite != nullptr) size = { static_cast<float>(sprite->GetWidth()) * scale, static_cast<float>(sprite->GetHeight()) * scale };
			else size = { 50, 50 };
			damage = 0;
		}

		void update(float _dt);

		void render() const override;
	};
}