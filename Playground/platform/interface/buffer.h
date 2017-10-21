#pragma once

#include "platform_constant.h"

namespace playground
{
	class IVertexBuffer
	{
	public:
		virtual ~IVertexBuffer() = 0
		{

		}

		virtual int GetLength() const = 0;
	};

	class IIndexBuffer
	{
	public:
		virtual ~IIndexBuffer() = 0
		{

		}

		virtual int GetLength() const = 0;
		virtual IndexBufferType GetType() const = 0;
	};
}