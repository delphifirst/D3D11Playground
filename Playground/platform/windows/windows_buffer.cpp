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
}