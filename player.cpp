#include "player.h"
#include "util.h"

namespace Tmpl8
{

	void Player::dash(vec2 _dir) // old mechanic, no longer used
	{
		dashTimer = 0;
		dashVel = _dir * 500;
		std::cout << "dashed" << std::endl;
	}

	void Player::update(float _dt)
	{
		// move player
		pos += (vel + dashVel) * _dt;
		dashVel *= pow(0.1f, _dt); // dampening

		// screen boundary collisions
		if (this->getTopLeft().x < 100) // LEFT
		{
			pos.x = 100 + size.x * scale / 2;
			vel = reflectVector(vel, { 1.0f, 0.0f });
			dashVel = reflectVector(dashVel, { 1.0f, 0.0f });
		}
		if (this->getBottomRight().x > ScreenWidth - 100) // RIGHT
		{
			pos.x = -100 + ScreenWidth - (size.x * scale / 2);
			vel = reflectVector(vel, { -1.0f, 0.0f });
			dashVel = reflectVector(dashVel, { -1.0f, 0.0f });
		}
		if (this->getTopLeft().y < 0 + 100) // TOP
		{
			pos.y = 100 + size.y * scale / 2;
			vel = reflectVector(vel, { 0.0f, 1.0f });
			dashVel = reflectVector(dashVel, { 0.0f, 1.0f });
		}
		if (this->getBottomRight().y > ScreenHeight - 100) // BOTTOM
		{
			pos.y = -100 + ScreenHeight - (size.y * scale / 2);
			vel = reflectVector(vel, { 0.0f, -1.0f });
			dashVel = reflectVector(dashVel, { 0.0f, -1.0f });
		}

		float angle = vec2ToAngle(vel);

		dashTimer += _dt;
		lastShotTimer += _dt;
		invincibility -= _dt;
		if (invincibility < 0) invincibility = 0;
	}

	// ran when player is hit by enemy
	bool Player::playerSubtractHealth()
	{
		invincibility = 2.0f;
		return --hp == 0; // check if player is dead
	}

	// renders a line in the direction the player is moving, which may help with aiming
	void Player::renderDirectionLine()
	{
		const vec3 normalizedVel = normalize({ vel.x, vel.y, 0 });
		screen->LineClipped({ pos.x, pos.y }, { pos.x + 2500 * normalizedVel.x, pos.y + 2500 * normalizedVel.y }, { 105, 105, ScreenWidth - 105, ScreenHeight - 105 }, 0x5df5ff);
	}


	void Player::render() const
	{
		//sprite->DrawScaled(static_cast<int>(pos.x), static_cast<int>(pos.y), size.x * scale, size.y * scale, screen, false);
		if (fmod(invincibility, 0.5) < 0.25)
		{
			screen->CircleShadow({ pos.x - 5, pos.y + 5 }, 35, 0.7f);
			screen->CircleFull(pos, 0, static_cast<int>(size.x / 2), 0x64b4ff);
			screen->CircleFull(pos, 20, 25, 0x5df5ff);
		}
	}
};







//#include "player.h"
//#include "util.h"
//
//#include <windows.h>
//#include <algorithm>
//
//namespace Tmpl8
//{
//	void Player::update(float _dt)
//	{
//		isRunning = false;
//		if (GetAsyncKeyState(VK_RIGHT))
//		{
//			vel.x = std::clamp(vel.x + acceleration * _dt, -velMax, velMax);
//			isRunning = true;
//		}
//		if (GetAsyncKeyState(VK_LEFT))
//		{
//			vel.x = std::clamp(vel.x - acceleration * _dt, -velMax, velMax);
//			isRunning = true;
//		}
//		if (GetAsyncKeyState(VK_UP))
//		{
//			vel.y = std::clamp(vel.y - acceleration * _dt, -velMax, velMax);
//			isRunning = true;
//		}
//		if (GetAsyncKeyState(VK_DOWN))
//		{
//			vel.y = std::clamp(vel.y + acceleration * _dt, -velMax, velMax);
//			isRunning = true;
//		}
//		doAnimation(_dt);
//
//		// slow down when appropriate
//		if (GetAsyncKeyState(VK_RIGHT) == GetAsyncKeyState(VK_LEFT) || (GetAsyncKeyState(VK_RIGHT) && vel.x < 0) || (GetAsyncKeyState(VK_LEFT) && vel.x > 0)) // slow down
//			vel.x = vel.x -= (vel.x * 10.0f) * (_dt);
//		if (GetAsyncKeyState(VK_UP) == GetAsyncKeyState(VK_DOWN) || (GetAsyncKeyState(VK_UP) && vel.y > 0) || (GetAsyncKeyState(VK_DOWN) && vel.y < 0))
//			vel.y = vel.y -= (vel.y * 10.0f) * (_dt);
//
//		// move player
//		pos.x += vel.x * _dt;
//		pos.y += vel.y * _dt;
//
//		// screen boundary collisions
//		if (this->getTopLeft().x < 0)
//		{
//			vel.x = 0;
//			pos.x = size.x * scale / 2;
//		}
//		if (this->getBottomRight().x > ScreenWidth)
//		{
//			vel.x = 0;
//			pos.x = ScreenWidth - (size.x * scale / 2);
//		}
//		if (this->getTopLeft().y < 0)
//		{
//			vel.y = 0;
//			pos.y = size.y * scale / 2;
//		}
//		if (this->getBottomRight().y > ScreenHeight)
//		{
//			vel.y = 0;
//			pos.y = ScreenHeight - (size.y * scale / 2);
//		}
//
//
//		lastShotTimer += _dt;
//	}
//
//	void Player::doAnimation(float _dt)
//	{
//		animationFrameDt += 3 * _dt;
//
//		if (isRunning && frame < animationMin && frame > animationMax) {
//			animationFrameDt = 6.0f;
//			animationMin = 6;
//			animationMax = 12;
//		}
//		else if (!isRunning && frame < animationMin && frame > animationMax)
//		{
//			animationFrameDt = 0.0f;
//			animationMin = 0;
//			animationMax = 5;
//		}
//
//		if (animationFrameDt > animationMax) { animationFrameDt = animationMin; }
//		frame = static_cast<int>(animationFrameDt);
//		animationFrameDt += 3 * _dt;
//	}
//
//	void Player::render()
//	{
//		sprite->SetFrame(frame);
//		sprite->DrawScaled(static_cast<int>(pos.x), static_cast<int>(pos.y), size.x * scale, size.y * scale, screen, false);
//	}
//};