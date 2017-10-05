#pragma once

#include <memory>

namespace playground
{
	class IDevice;

	class IRenderObject
	{
	public:
		virtual void Draw(std::shared_ptr<IDevice> device) = 0;
	};
}