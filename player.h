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
		float hp = 100;

		float velMax = 450.0f;
		float acceleration = 1500.0f;
		float lastShot = 0;
		float shotDelay = 0.05f;
		float scale = 0.4f; // factor to multiply the sprite size with
		int animationMin = 0; // idle: 0, walk: 6
		int animationMax = 5; // idle: 5, walk: 12
		float animationFrameDt = 0.0f;
		bool isRunning = false;

	public:
		vec2 getTopLeft() { return { pos.x - (size.x * scale / 2), pos.y - (size.y * scale / 2) }; }
		vec2 getBottomRight() { return { pos.x + (size.x * scale / 2), pos.y + (size.y * scale / 2) }; }
		void setWeapon(int _weapon) { weapon = _weapon; }
		int getWeapon() const { return weapon; }
		void setHp(float _hp) { hp = _hp; }
		float getHp() const { return hp; }
		bool canShoot() const { return (lastShot > shotDelay); }
		void resetShotTimer() { lastShot = 0; }
		

		Player(vec2 _pos, Surface* _screen, std::unique_ptr<Sprite> _sprite) :
			Entity(_pos, _screen, std::move(_sprite))
		{
			vel = { 0, 0 };
			size = { static_cast<float>(sprite->GetWidth()) * scale, static_cast<float>(sprite->GetHeight()) * scale};
			damage = 0;
		}

		void doAnimation(float _dt);

		void update(float _dt);

		void render();
	};
}