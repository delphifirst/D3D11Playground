#include "utils.h"

void AssertSucceed(HRESULT hr, const std::wstring& message)
{
	if (FAILED(hr))
		throw AppError(message);
}