#pragma once

#include <memory>

namespace playground
{
	class MainRenderer;
	class Camera;

	class IRenderObject
	{
	public:
		virtual void PreDraw(MainRenderer* main_renderer) = 0;
		virtual void Draw(MainRenderer* main_renderer, Camera* camera) = 0;
	};
}