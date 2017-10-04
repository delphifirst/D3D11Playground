#pragma once

#include <memory>

namespace playground
{
	class IDevice;

	class IRenderer
	{
	public:
		virtual void Init(std::shared_ptr<IDevice> device) = 0;
		virtual std::shared_ptr<IDevice> GetDevice() = 0;
		virtual void Render(float delta_time) = 0;
	};
}