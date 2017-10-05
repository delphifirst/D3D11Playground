#pragma once

#include <cmath>
#include <cassert>

namespace playground
{
	template<typename T>
	struct TVec3;

	template<typename T>
	struct TVec4
	{
		// Data
		T x, y, z, w;

		// Constructors
		TVec4() : x(0), y(0), z(0), w(0)
		{

		}
		TVec4(T ax, T ay, T az, T aw) : x(ax), y(ay), z(az), w(aw)
		{

		}
		TVec4(const TVec3<T>& other, T w)
			: x(other.x), y(other.y), z(other.z), w(w)
		{

		}

		// Operators
		const T& operator[](int index) const
		{
			if (index == 0)
				return x;
			else if (index == 1)
				return y;
			else if (index == 2)
				return z;
			else if (index == 3)
				return w;
			else
				assert(0);
		}
		T& operator[](int index)
		{
			return const_cast<T&>(static_cast<const TVec4&>(*this)[index]);
		}
		const TVec4 operator-() const
		{
			return TVec4(-x, -y, -z, -w);
		}
		const TVec4 operator+() const
		{
			return *this;
		}
		TVec4& operator+=(const TVec4& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			w += rhs.w;
			return *this;
		}
		TVec4& operator-=(const TVec4& rhs)
		{
			return *this += -rhs;
		}
		TVec4& operator*=(T rhs)
		{
			x *= rhs;
			y *= rhs;
			z *= rhs;
			w *= rhs;
			return *this;
		}
		TVec4& operator/=(T rhs)
		{
			x /= rhs;
			y /= rhs;
			z /= rhs;
			w /= rhs;
			return *this;
		}

		// Functions
		T SquareLength() const
		{
			return x * x + y * y + z * z + w * w;
		}
		T Length() const
		{
			return sqrt(SquareLength());
		}
		const TVec4 Normalized() const
		{
			return *this / Length();
		}
	};

	// TVec4 operators
	template<typename T>
	const TVec4<T> operator+(const TVec4<T>& lhs, const TVec4<T>& rhs)
	{
		TVec4<T> result = lhs;
		result += rhs;
		return result;
	}
	template<typename T>
	const TVec4<T> operator-(const TVec4<T>& lhs, const TVec4<T>& rhs)
	{
		return lhs + (-rhs);
	}
	template<typename T>
	const TVec4<T> operator*(T lhs, const TVec4<T>& rhs)
	{
		TVec4<T> result = rhs;
		result *= lhs;
		return result;
	}
	template<typename T>
	const TVec4<T> operator*(const TVec4<T>& lhs, T rhs)
	{
		return rhs * lhs;
	}
	template<typename T>
	const TVec4<T> operator/(const TVec4<T>& lhs, T rhs)
	{
		TVec4<T> result = lhs;
		result /= rhs;
		return result;
	}

	// TVec4 functions
	template<typename T>
	T Dot(const TVec4<T>& lhs, const TVec4<T>& rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
	}

	// TVec4 typedefs
	typedef TVec4<double> Vec4d;
	typedef TVec4<float> Vec4f;


	template<typename T>
	struct TVec3
	{
		// Data
		T x, y, z;

		// Constructors
		TVec3() : x(0), y(0), z(0)
		{

		}
		TVec3(T ax, T ay, T az) : x(ax), y(ay), z(az)
		{

		}
		explicit TVec3(const TVec4<T>& other)
			: x(other.x), y(other.y), z(other.z)
		{

		}

		// Operators
		const T& operator[](int index) const
		{
			if (index == 0)
				return x;
			else if (index == 1)
				return y;
			else if (index == 2)
				return z;
			else
				assert(0);
		}
		T& operator[](int index)
		{
			return const_cast<T&>(static_cast<const TVec3&>(*this)[index]);
		}
		const TVec3 operator-() const
		{
			return TVec3(-x, -y, -z);
		}
		const TVec3 operator+() const
		{
			return *this;
		}
		TVec3& operator+=(const TVec3& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			return *this;
		}
		TVec3& operator-=(const TVec3& rhs)
		{
			return *this += -rhs;
		}
		TVec3& operator*=(T rhs)
		{
			x *= rhs;
			y *= rhs;
			z *= rhs;
			return *this;
		}
		TVec3& operator/=(T rhs)
		{
			x /= rhs;
			y /= rhs;
			z /= rhs;
			return *this;
		}

		// Functions
		T SquareLength() const
		{
			return x * x + y * y + z * z;
		}
		T Length() const
		{
			return sqrt(SquareLength());
		}
		const TVec3 Normalized() const
		{
			return *this / Length();
		}
	};

	// TVec3 operators
	template<typename T>
	const TVec3<T> operator+(const TVec3<T>& lhs, const TVec3<T>& rhs)
	{
		TVec3<T> result = lhs;
		result += rhs;
		return result;
	}
	template<typename T>
	const TVec3<T> operator-(const TVec3<T>& lhs, const TVec3<T>& rhs)
	{
		return lhs + (-rhs);
	}
	template<typename T>
	const TVec3<T> operator*(T lhs, const TVec3<T>& rhs)
	{
		TVec3<T> result = rhs;
		result *= lsh;
		return result;
	}
	template<typename T>
	const TVec3<T> operator*(const TVec3<T>& lhs, T rhs)
	{
		return rhs * lhs;
	}
	template<typename T>
	const TVec3<T> operator/(const TVec3<T>& lhs, T rhs)
	{
		TVec3<T> result = lhs;
		result /= rhs;
		return result;
	}

	// TVec3 functions
	template<typename T>
	T Dot(const TVec3<T>& lhs, const TVec3<T>& rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
	}
	template<typename T>
	const TVec3<T> Cross(const TVec3<T>& lhs, const TVec3<T>& rhs)
	{
		return TVec3<T>(lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x);
	}

	// TVec3 typedefs
	typedef TVec3<double> Vec3d;
	typedef TVec3<float> Vec3f;
}