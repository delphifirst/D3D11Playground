#pragma once

#include <string>

#include <Windows.h>

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

void AssertSucceed(HRESULT hr, const std::wstring& message);