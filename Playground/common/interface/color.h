#pragma once

#include <cstdint>

namespace playground
{
	struct Color4b
	{
		uint8_t r, g, b, a;

		Color4b() : r(0), g(0), b(0), a(0)
		{

		}

		Color4b(uint8_t ar, uint8_t ag, uint8_t ab, uint8_t aa) : r(ar), g(ag), b(ab), a(aa)
		{

		}
	};
}