#pragma once

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
}