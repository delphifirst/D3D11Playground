#pragma once

#include <d3d11.h>

#include "buffer.h"

namespace playground
{
	class WindowsVertexBuffer :public IVertexBuffer
	{
	public:
		WindowsVertexBuffer(int length, ID3D11Buffer* buffer);
		virtual ~WindowsVertexBuffer() override;

		virtual int GetLength() const override
		{
			return length_;
		}

		ID3D11Buffer* GetBuffer()
		{
			return buffer_;
		}
	private:
		int length_;
		ID3D11Buffer* buffer_;
	};
}