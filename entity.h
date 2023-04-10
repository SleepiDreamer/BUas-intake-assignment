#pragma once
#include <memory>
#include <utility>
#include "surface.h"
#include "template.h"

namespace Tmpl8 {
	class Entity
	{
	protected:
		vec2 pos;
		vec2 vel;
		vec2 dir; // radians
		vec2 size;
		Surface* screen;
		float damage;
		std::shared_ptr<Sprite> sprite; // default constructed to nullptr
		int frame = 0;
	public:
		void setPos( vec2 _pos ) { pos = _pos; }
		vec2 getPos() { return pos; }
		vec2 getTopLeft() { return { pos.x - (size.x / 2), pos.y - (size.y / 2) }; }
		virtual vec2 getBottomRight() { return { pos.x + (size.x / 2), pos.y + (size.y / 2) }; }
		void setVel( vec2 _vel ) { vel = _vel; }
		vec2 getVel() { return vel; }
		void setDir( vec2 _pos ) { dir = _pos; }
		vec2 getDir() { return dir; }
		void setSize( vec2 _size ) { size = _size; }
		vec2 getSize() { return size; }
		void setDamage( float _damage ) { damage = _damage; }
		float getDamage() { return damage; }
		void setSprite(std::shared_ptr<Sprite> _sprite ) { sprite = std::move(_sprite); }
		const Sprite& getSprite() { return *sprite; }
		void setFrame(int _frame ) { frame = _frame; }
		int getFrame() { return frame; }

		Entity(vec2 _pos, Surface* _screen, std::shared_ptr<Sprite> _sprite) :
			pos(_pos), screen(_screen), sprite(std::move(_sprite))
		{
			dir = { 0, 0 };
			vel = { 0, 0 };
			size = { static_cast<float>(sprite->GetWidth()), static_cast<float>(sprite->GetHeight()) };
			damage = 0;
		}

		

		virtual void render() const
		{
			sprite->SetFrame(frame);
			sprite->Draw(screen, pos.x - (size.x / 2), pos.y - (size.y / 2));
		}

		/**
		 * \brief sets an enemy's rotation to point towards a coordinate
		 * \param px  x coordinate to point towards (screen-space)
		 * \param py  y coordinate to point towards (screen-space)
		 */
		void PointTowards(vec2 _pos);

		/**
		 * \brief moves the player towards a specified coordinate (screen-space)
		 * \param px  x coordinate to move towards
		 * \param py  y coordinate to move towards
		 * \param dist  how far to move
		 */
		void MoveTowards(vec2 _pos, const float _dist);
	};
}
