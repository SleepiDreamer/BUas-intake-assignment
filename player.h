#pragma once
#include <iostream>
#include <vector>

#include "surface.h"
#include "template.h"
#include "entity.h"
#include "util.h"


namespace Tmpl8 {
	class Player : public Entity
	{
	private:
		vec2 dashVel = { 0, 0 };
		int weapon = 0;
		int maxHp = 3;
		int hp = maxHp;
		float invincibility = 0.0f;
		float lastShotTimer = 0.0f;
		float shotDelay = 0.05f;
		float scale = 1.0f; // factor to multiply the sprite size with
		float dashTimer = 0.0f;
		float dashDelay = 1.0f;

	public:
        vec2 getTopLeft() override { return { pos.x - size.x * scale / 2, pos.y - size.y * scale / 2 }; }
        vec2 getBottomRight() override { return { pos.x + size.x * scale / 2, pos.y + size.y * scale / 2 }; }
		void setWeapon(int _weapon) { weapon = _weapon; }
		int getWeapon() const { return weapon; }
		void setHp(int _hp) { hp = _hp; }
		int getHp() const { return hp; }
		int getMaxHp() const { return maxHp; }
		bool canShoot() const { return lastShotTimer > shotDelay; }
		bool canShootFast() const { return lastShotTimer > shotDelay / 2; }
		void resetShotTimer() { lastShotTimer = 0; }
		void setInvincibility(float _invincibility) { invincibility	= _invincibility; }
		float getInvincibility() const { return invincibility; }
		bool playerSubtractHealth();
		void setShotDelay(float _shotDelay) { shotDelay = _shotDelay; }
		float getShotDelay() const { return shotDelay; }
		bool canDash() const { return dashTimer >= dashDelay; }

		Player(vec2 _pos, Surface* _screen, std::unique_ptr<Sprite> _sprite) :
			Entity(_pos, _screen, std::move(_sprite))
		{
			vel = { -300, 100 };
			//vel = { 0, 0 };
			if (sprite != nullptr) size = { static_cast<float>(sprite->GetWidth()) * scale, static_cast<float>(sprite->GetHeight()) * scale };
			else size = { 50, 50 };
			damage = 0;
		}

		void dash(vec2 _dir);

		void update(float _dt);

		void render() const override;
	};
}
