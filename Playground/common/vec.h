#pragma once

namespace playground
{
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

		// Operators
		TVec4 operator-() const
		{
			return TVec4(-x, -y, -z, -w);
		}
		TVec4 operator+() const
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
	};

	// Vec4 operators
	template<typename T>
	TVec4<T> operator+(const TVec4<T>& lhs, const TVec4<T>& rhs)
	{
		return TVec4<T>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
	}
	template<typename T>
	TVec4<T> operator-(const TVec4<T>& lhs, const TVec4<T>& rhs)
	{
		return lhs + (-rhs);
	}
	template<typename T>
	TVec4<T> operator*(T lhs, const TVec4<T>& rhs)
	{
		return TVec4<T>(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w);
	}
	template<typename T>
	TVec4<T> operator*(const TVec4<T>& lhs, T rhs)
	{
		return rhs * lhs;
	}
	template<typename T>
	TVec4<T> operator/(const TVec4<T>& lhs, T rhs)
	{
		return TVec4<T>(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs);
	}

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

		// Operators
		TVec3 operator-() const
		{
			return TVec3(-x, -y, -z);
		}
		TVec3 operator+() const
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
	};

	// Vec3 operators
	template<typename T>
	TVec3<T> operator+(const TVec3<T>& lhs, const TVec3<T>& rhs)
	{
		return TVec3<T>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
	}
	template<typename T>
	TVec3<T> operator-(const TVec3<T>& lhs, const TVec3<T>& rhs)
	{
		return lhs + (-rhs);
	}
	template<typename T>
	TVec3<T> operator*(T lhs, const TVec3<T>& rhs)
	{
		return TVec3<T>(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
	}
	template<typename T>
	TVec3<T> operator*(const TVec3<T>& lhs, T rhs)
	{
		return rhs * lhs;
	}
	template<typename T>
	TVec3<T> operator/(const TVec3<T>& lhs, T rhs)
	{
		return TVec3<T>(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
	}

	typedef TVec3<double> Vec3d;
	typedef TVec3<float> Vec3f;
}