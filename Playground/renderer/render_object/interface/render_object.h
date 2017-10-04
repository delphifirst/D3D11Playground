#pragma once

#include <memory>

namespace playground
{
	class MainRenderer;

	class RenderObject
	{
	public:
		RenderObject(std::shared_ptr<MainRenderer> main_renderer);
		virtual void Draw() = 0;
	protected:
		std::shared_ptr<MainRenderer> main_renderer_;
	};
}