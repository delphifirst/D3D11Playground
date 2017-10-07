#pragma once

#include <string>

#include "platform_constant.h"

namespace playground
{
	struct InputElementDesc
	{
		std::string name;
		VertexFormat format;
		int slot;
		int byte_offset;
	};
	class IInputLayout
	{
	public:
		virtual ~IInputLayout() = 0
		{

		}
	};
}