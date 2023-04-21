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
		virtual ~Entity() = default;
		void setPos( vec2 _pos ) { pos = _pos; }
		vec2 getPos() const { return pos; }
		virtual vec2 getTopLeft() { return { pos.x - (size.x / 2), pos.y - (size.y / 2) }; }
		virtual vec2 getBottomRight() { return { pos.x + (size.x / 2), pos.y + (size.y / 2) }; }
		void setVel( vec2 _vel ) { vel = _vel; }
		vec2 getVel() const { return vel; }
		void setDir( vec2 _pos ) { dir = _pos; }
		vec2 getDir() const { return dir; }
		void setSize( vec2 _size ) { size = _size; }
		vec2 getSize() const { return size; }
		void setDamage( float _damage ) { damage = _damage; }
		float getDamage() const { return damage; }
		void setSprite(std::shared_ptr<Sprite> _sprite ) { sprite = std::move(_sprite); }
		const Sprite& getSprite() const { return *sprite; }
		void setFrame(int _frame ) { frame = _frame; }
		int getFrame() const { return frame; }

		Entity(vec2 _pos, Surface* _screen, std::shared_ptr<Sprite> _sprite) :
			pos(_pos), screen(_screen), sprite(std::move(_sprite))
		{
			dir = { 0, 0 };
			vel = { 0, 0 };
			if (sprite != nullptr) size = { static_cast<float>(sprite->GetWidth()), static_cast<float>(sprite->GetHeight()) };
			else size = { 50, 50 };
			damage = 0;
		}

		

		virtual void render() const
		{
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
		void MoveTowards(vec2 _pos, float _speed);
	};
}
