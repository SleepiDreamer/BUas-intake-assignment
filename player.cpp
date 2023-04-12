#include "player.h"
#include "util.h"

#include <windows.h>
#include <algorithm>

namespace Tmpl8
{
	void Player::update(float _dt)
	{
		// move player
		pos += vel * _dt;

		// screen boundary collisions
		if (this->getTopLeft().x < 0) // LEFT
		{
			pos.x = size.x * scale / 2;
			vel = reflectVector(vel, { 1.0f, 0.0f });
		}
		if (this->getBottomRight().x > ScreenWidth) // RIGHT
		{
			pos.x = ScreenWidth - (size.x * scale / 2);
			vel = reflectVector(vel, { -1.0f, 0.0f });
		}
		if (this->getTopLeft().y < 0) // TOP
		{
			pos.y = size.y * scale / 2;
			vel = reflectVector(vel, { 0.0f, 1.0f });
		}
		if (this->getBottomRight().y > ScreenHeight) // BOTTOM
		{
			pos.y = ScreenHeight - (size.y * scale / 2);
			vel = reflectVector(vel, { 0.0f, -1.0f });
		}

		float angle = vec2ToAngle(vel);

		lastShot += _dt;
	}

	void Player::render()
	{
		sprite->SetFrame(frame);
		sprite->DrawScaled(static_cast<int>(pos.x), static_cast<int>(pos.y), size.x * scale, size.y * scale, screen, false);
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
//		lastShot += _dt;
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