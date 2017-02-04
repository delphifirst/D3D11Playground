#pragma once

#include <string>

class AppError
{
public:
	AppError(std::wstring reason) : reason_(reason)
	{
	}

	std::wstring reason() const
	{
		return reason_;
	}
private:
	std::wstring reason_;
};