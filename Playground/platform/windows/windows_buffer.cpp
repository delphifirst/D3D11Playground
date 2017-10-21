#include "windows_buffer.h"

#include "utils.h"

namespace playground
{
	WindowsVertexBuffer::WindowsVertexBuffer(int length, ID3D11Buffer* buffer)
		:length_(length), buffer_(buffer)
	{

	}

	WindowsVertexBuffer::~WindowsVertexBuffer()
	{
		SafeRelease(buffer_);
	}

	WindowsIndexBuffer::WindowsIndexBuffer(int length, IndexBufferType type, ID3D11Buffer* buffer)
		:length_(length), type_(type), buffer_(buffer)
	{

	}

	WindowsIndexBuffer::~WindowsIndexBuffer()
	{
		SafeRelease(buffer_);
	}
}