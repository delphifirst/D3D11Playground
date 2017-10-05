#pragma once

#include <vector>
#include <string>
#include <fstream>

template<typename T>
void SafeRelease(T* &obj)
{
	if (obj)
	{
		obj->Release();
		obj = nullptr;
	}
}

std::vector<char> ReadFile(const std::string& filename);