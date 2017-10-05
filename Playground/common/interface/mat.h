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

		// Operators
		const T& operator()(int row, int col) const
		{
			return cols[col][row];
		}
		T& operator()(int row, int col)
		{
			return const_cast<T&>(static_cast<const TMat4x4&>(*this)(row, col));
		}
		const TMat4x4 operator-() const
		{
			return TMat4x4(-cols[0], -cols[1], -cols[2], -cols[3]);
		}
		const TMat4x4 operator+() const
		{
			return *this;
		}
		TMat4x4& operator+=(const TMat4x4& rhs)
		{
			cols[0] += rhs.cols[0];
			cols[1] += rhs.cols[1];
			cols[2] += rhs.cols[2];
			cols[3] += rhs.cols[3];
			return *this;
		}
		TMat4x4& operator-=(const TMat4x4& rhs)
		{
			return *this += -rhs;
		}
		TMat4x4& operator*=(T rhs)
		{
			cols[0] *= rhs;
			cols[1] *= rhs;
			cols[2] *= rhs;
			cols[3] *= rhs;
			return *this;
		}
		TMat4x4& operator/=(T rhs)
		{
			cols[0] /= rhs;
			cols[1] /= rhs;
			cols[2] /= rhs;
			cols[3] /= rhs;
			return *this;
		}

		// Functions
		T Cofactor(int row, int col) const
		{
			T sign = (row + col) % 2 == 0 ? 1 : -1;
			T remain_mat[9];
			for (int i = 0, src_row = 0; i < 3; ++src_row)
			{
				if (src_row == row)
					continue;
				for (int j = 0, src_col = 0; j < 3; ++src_col)
				{
					if (src_col == col)
						continue;
					remain_mat[3 * i + j] = mat4.get(m, src_row, src_col);
					++j;
				}
				++i;
			}
			T remain_mat_det = remain_mat[0] * remain_mat[4] * remain_mat[8]
				+ remain_mat[2] * remain_mat[3] * remain_mat[7]
				+ remain_mat[6] * remain_mat[1] * remain_mat[5]
				- remain_mat[2] * remain_mat[4] * remain_mat[6]
				- remain_mat[0] * remain_mat[5] * remain_mat[7]
				- remain_mat[8] * remain_mat[3] * remain_mat[1];
			return sign * remain_mat_det;
		}
		T Det() const
		{
			T result = 0;
			for (int i = 0; i < 4; ++i)
				result += (*this)(i, 0) * Cofactor(i, 0);
			return result;
		}
		const TMat4x4 Transposed() const
		{
			TMat4x4 result;
			for (int i = 0; i < 4; ++i)
				for (int j = 0; j < 4; ++j)
					result(i, j) = (*this)(j, i);
			return result;
		}
		const TMat4x4 Invert() const
		{
			TMat4x4 cofactor_matrix;
			for (int i = 0; i < 4; ++i)
				for (int j = 0; j < 4; ++j)
					cofactor_matrix(i, j) = Cofactor(i, j);
			cofactor_matrix = cofactor_matrix.Transposed();
			return cofactor_matrix / Det();
		}

		// Matrix creators
		static const TMat4x4 Identity()
		{
			return TMat4x4(
				TVec4<T>(1, 0, 0, 0),
				TVec4<T>(0, 1, 0, 0),
				TVec4<T>(0, 0, 1, 0),
				TVec4<T>(0, 0, 0, 1));
		}
		static const TMat4x4 LookAt(const TVec3<T>& eye, const TVec3<T>& focus, const TVec3<T>& up)
		{
			TVec3<T> w = (eye - focus).Normalized();
			TVec3<T> u = Cross(up, w).Normalized();
			TVec3<T> v = Cross(w, u);
			return TMat4x4(TVec4<T>(u, 0), TVec4<T>(v, 0), TVec4<T>(w, 0), TVec4<T>(eye, 1));
		}
		static const TMat4x4 Perspective(T fovy, T aspect, T near, T far)
		{
			T f = 1.0 / tan(fovy / 2.0), nf = 1.0 / (near - far);
			return TMat4x4(
				TVec4<T>(f / aspect, 0, 0, 0), 
				TVec4<T>(0, f, 0, 0), 
				TVec4<T>(0, 0, (far + near) * nf, -1), 
				TVec4<T>(0, 0, (2 * far * near) * nf, 0));
		}
	};

	// TMat4x4 operators
	template<typename T>
	const TMat4x4<T> operator+(const TMat4x4<T>& lhs, const TMat4x4<T>& rhs)
	{
		TMat4x4<T> result = lhs;
		result += rhs;
		return result;
	}
	template<typename T>
	const TMat4x4<T> operator-(const TMat4x4<T>& lhs, const TMat4x4<T>& rhs)
	{
		return lhs + (-rhs);
	}
	template<typename T>
	const TMat4x4<T> operator*(T lhs, const TMat4x4<T>& rhs)
	{
		TMat4x4<T> result = rhs;
		result *= lhs;
		return result;
	}
	template<typename T>
	const TMat4x4<T> operator*(const TMat4x4<T>& lhs, T rhs)
	{
		return rhs * lhs;
	}
	template<typename T>
	const TMat4x4<T> operator/(const TMat4x4<T>& lhs, T rhs)
	{
		TMat4x4<T> result = lhs;
		result /= rhs;
		return result;
	}
	template<typename T>
	const TVec4<T> operator*(const TMat4x4<T>& lhs, const TVec4<T>& rhs)
	{
		return lhs.cols[0] * rhs.x + lhs.cols[1] * rhs.y + lhs.cols[2] * rhs.z + lhs.cols[3] * rhs.w;
	}
	template<typename T>
	const TMat4x4<T> operator*(const TMat4x4<T>& lhs, const TMat4x4<T>& rhs)
	{
		return TMat4x4<T>(lhs * rhs.cols[0], lhs * rhs.cols[1], lhs * rhs.cols[2], lhs * rhs.cols[3]);
	}

	typedef TMat4x4<double> Mat4x4d;
	typedef TMat4x4<float> Mat4x4f;
}