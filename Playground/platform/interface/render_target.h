#pragma once

namespace playground
{
	class IRenderTarget
	{
	public:
		virtual ~IRenderTarget() = 0
		{

		}
		virtual int GetWidth() const = 0;
		virtual int GetHeight() const = 0;
	};
}