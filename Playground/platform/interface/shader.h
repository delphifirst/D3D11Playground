#pragma once

#include <string>

namespace playground
{
	enum ShaderType
	{
		UNKNOWN_SHADER,
		VERTEX_SHADER,
		PIXEL_SHADER
	};

	class IShader
	{
	public:
		virtual ~IShader() = 0
		{

		}

		virtual std::string GetName() const = 0;
		virtual ShaderType GetType() const = 0;
	};
}