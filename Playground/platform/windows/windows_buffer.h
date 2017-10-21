#pragma once

#include <d3d11.h>

#include "buffer.h"
#include "platform_constant.h"

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

	class WindowsIndexBuffer :public IIndexBuffer
	{
	public:
		WindowsIndexBuffer(int length, IndexBufferType type, ID3D11Buffer* buffer);
		virtual ~WindowsIndexBuffer() override;

		virtual int GetLength() const override
		{
			return length_;
		}

		ID3D11Buffer* GetBuffer()
		{
			return buffer_;
		}

		virtual IndexBufferType GetType() const override
		{
			return type_;
		}
	private:
		int length_;
		IndexBufferType type_;
		ID3D11Buffer* buffer_;
	};
}