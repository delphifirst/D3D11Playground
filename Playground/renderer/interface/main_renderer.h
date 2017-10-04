#pragma once

#include <memory>
#include <vector>

#include "renderer.h"

namespace playground
{
	class RenderObject;

	class MainRenderer :public IRenderer
	{
	public:
		virtual void Init(std::shared_ptr<IDevice> device) override;
		virtual std::shared_ptr<IDevice> GetDevice() override
		{
			return device_;
		}
		virtual void Render(float delta_time) override;

		void AddRenderObject(std::shared_ptr<RenderObject> render_object);
	private:
		std::shared_ptr<IDevice> device_;

		std::vector<std::shared_ptr<RenderObject>> render_objects_;
	};
}