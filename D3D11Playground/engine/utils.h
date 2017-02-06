#pragma once

#include <string>

#include <Windows.h>
#include <d3d11.h>

enum class ShaderType
{
	VS, PS, DS, HS, CS
};

class AppError
{
public:
	AppError(const std::wstring& reason) : reason_(reason)
	{
	}

	std::wstring reason() const
	{
		return reason_;
	}
private:
	std::wstring reason_;
};

template<typename T>
void SafeRelease(T* &obj)
{
	if (obj)
	{
		obj->Release();
		obj = nullptr;
	}
}

int GetPixelSize(DXGI_FORMAT format);