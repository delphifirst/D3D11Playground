#pragma once

#include "vec.h"

namespace playground
{
	template<typename T>
	struct TMat4x4
	{
		// Data
		TVec4<T> cols[4];
		
		// Constructors
		TMat4x4()
		{

		}
		TMat4x4(const TVec4<T>& c0, const TVec4<T>& c1, const TVec4<T>& c2, const TVec4<T>& c3)
		{
			cols[0] = c0;
			cols[1] = c1;
			cols[2] = c2;
			cols[3] = c3;
		}
	};

	typedef TMat4x4<double> Mat4x4d;
	typedef TMat4x4<float> Mat4x4f;
}