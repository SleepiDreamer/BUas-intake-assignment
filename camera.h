#pragma once
#include "surface.h"
#include "template.h"

namespace Tmpl8 {
	class Camera
	{
	protected:
		float Zoom;
		vec2 ScreenshakeOffset;
	public:
		void setZoom(float zoom) { Zoom = zoom; }
		float getZoom() const { return Zoom; }
		void setScreenshakeOffset(vec2 screenshakeOffset) { ScreenshakeOffset = screenshakeOffset; }
		vec2 getScreenshakeOffset() const { return ScreenshakeOffset; }


		Camera(vec2 _pos, vec2 _vel, vec2 _size, Surface* _screen, float _zoom, vec2 _screenshakeOffset)
		{
			Zoom = _zoom;
			ScreenshakeOffset = _screenshakeOffset;
		}
	};
}
