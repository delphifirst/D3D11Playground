#pragma once

#include <memory>
#include <vector>

namespace playground
{
	class IRenderObject;
	class IDevice;

	class MainRenderer
	{
	public:
		virtual void Init(std::shared_ptr<IDevice> device);
		std::shared_ptr<IDevice> GetDevice()
		{
			return device_;
		}
		virtual void Render(float delta_time);

		void AddRenderObject(std::shared_ptr<IRenderObject> render_object);
	private:
		std::shared_ptr<IDevice> device_;

		std::vector<std::shared_ptr<IRenderObject>> render_objects_;
	};
}