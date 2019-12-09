#pragma once
#ifndef JKMATH_H_
#define JKMATH_H_

#include<math.h>
#include<iostream>

typedef unsigned int UINT;

namespace jkMath
{
	const float PI = 3.141592653f;

	struct VEC2;
	struct VEC3;
	struct VEC4;

	struct VEC2
	{
		VEC2() { x = y = 0.0f; };
		VEC2(float _x, float _y) :x(_x), y(_y) {};

		bool operator==(const VEC2& _vec) const
		{
			return (x == _vec.x && y == _vec.y);
		}

		VEC2 operator+(const VEC2& _vec) const
		{
			return VEC2(x + _vec.x, y + _vec.y);
		}

		VEC2 operator-(const VEC2& _vec) const
		{
			return VEC2(x - _vec.x, y - _vec.y);
		}

		VEC2 operator*(const float scaleFactor) const
		{
			return VEC2(x*scaleFactor, y*scaleFactor);
		}

		friend VEC2 operator*(const float scaleFactor, VEC2 _vec)
		{
			return _vec * scaleFactor;
		}

		VEC2 operator/(const float scaleFactor) const
		{
			return VEC2(x / scaleFactor, y / scaleFactor);
		}

		float x, y;
	};

	struct VEC3
	{
		VEC3() : x(0), y(0), z(0) {};
		VEC3(const VEC3& _vec) :x(_vec.x), y(_vec.y), z(_vec.z) {};
		VEC3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {};
		VEC3(const float column[3])
		{
			x = column[0];
			y = column[1];
			z = column[2];
		}
		VEC3(float _v) : VEC3(_v, _v, _v) {};
		VEC3(const VEC2& _vec2, float _z) : VEC3(_vec2.x, _vec2.y, _z) {};

		float Length() const
		{
			return sqrt(x*x + y * y + z * z);
		}

		VEC3 Normalize() const
		{
			float length = this->Length();
			if (length == 0)
			{
				return VEC3(0, 0, 0);
			}
			else
			{
				return VEC3(x / length, y / length, z / length);
			}
		}

		bool operator==(const VEC3& _vec) const
		{
			return (x == _vec.x && y == _vec.y && z == _vec.z);
		}

		const VEC3 operator+(const VEC3& _vec) const
		{
			return VEC3(x + _vec.x, y + _vec.y, z + _vec.z);
		}

		VEC3& operator+=(const VEC3& _vec)
		{
			x += _vec.x;
			y += _vec.y;
			z += _vec.z;

			return *this;
		}

		VEC3 operator-(const VEC3& _vec) const
		{
			return VEC3(x - _vec.x, y - _vec.y, z - _vec.z);
		}

		VEC3 operator-()const
		{
			return VEC3(-x, -y, -z);
		}

		VEC3 operator*(const float scaleFactor) const
		{
			return VEC3(x*scaleFactor, y*scaleFactor, z*scaleFactor);
		}

		friend VEC3 operator*(const float scaleFactor, const VEC3 _vec)
		{
			return _vec * scaleFactor;
		}

		VEC3 operator*(const VEC3& _vec) const
		{
			return VEC3(x*_vec.x, y*_vec.y, z*_vec.z);
		}

		VEC3& operator*=(const float scaleFactor)
		{
			x *= scaleFactor;
			y *= scaleFactor;
			z *= scaleFactor;

			return *this;
		}

		VEC3 operator/(const float scaleFactor) const
		{
			return VEC3(x / scaleFactor, y / scaleFactor, z / scaleFactor);
		}

		VEC3 CrossProduct(const VEC3& _vec) const
		{
			return VEC3(_vec.z*y - _vec.y*z, _vec.x*z - _vec.z*x, _vec.y*x - _vec.x*y);
		}

		float DotProduct(const VEC3& _vec) const
		{
			return _vec.x*x + _vec.y*y + _vec.z*z;
		}

		float CosineValue(const VEC3& _vec) const
		{
			return (*this).Normalize().DotProduct(_vec.Normalize());
		}

		friend std::ostream& operator<<(std::ostream& os, const VEC3 _vec)
		{
			return os << _vec.x << "," << _vec.y << "," << _vec.z << std::endl;
		}

		union {
			struct { float x, y, z; };
			float m[3];
		};
	};

	struct VEC4
	{
		VEC4() :x(0), y(0), z(0), w(0) {};
		VEC4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {};
		VEC4(float _v) :x(_v), y(_v), z(_v), w(_v) {};
		VEC4(const VEC3& _vec3, float _w) : x(_vec3.x), y(_vec3.y), z(_vec3.z), w(_w) {};
		VEC4(const float column[4])
		{
			x = column[0];
			y = column[1];
			z = column[2];
			w = column[3];
		}

		const float(&GetValue() const)[4]
		{
			float array[4] = { x, y, z, w };
			return array;
		}

			VEC3 xyz() const
		{
			return VEC3(x, y, z);
		}

		float Length() const
		{
			return sqrt(x*x + y * y + z * z + w * w);
		}

		VEC4& Normalize()
		{
			float length = this->Length();
			if (length == 0)
			{
				x = y = z = w = 0.0f;
			}
			else
			{
				x /= length;
				y /= length;
				z /= length;
				w /= length;
			}

			return *this;
		}

		bool operator==(const VEC4& _vec) const
		{
			return (x == _vec.x && y == _vec.y && z == _vec.z && w == _vec.w);
		}

		VEC4 operator+(const VEC4& _vec) const
		{
			return VEC4(x + _vec.x, y + _vec.y, z + _vec.z, w + _vec.w);
		}

		VEC4 operator-(const VEC4& _vec) const
		{
			return VEC4(x - _vec.x, y - _vec.y, z - _vec.z, w - _vec.w);
		}

		VEC4 operator*(const float scaleFactor) const
		{
			return VEC4(x*scaleFactor, y*scaleFactor, z*scaleFactor, w*scaleFactor);
		}

		friend VEC4 operator*(const float scaleFactor, VEC4 _vec)
		{
			return _vec * scaleFactor;
		}

		float operator*(const VEC4 _vec) const
		{
			return (x*_vec.x + y * _vec.y + z * _vec.z + w * _vec.w);
		}

		VEC4 operator/(const float scaleFactor) const
		{
			return VEC4(x / scaleFactor, y / scaleFactor, z / scaleFactor, w / scaleFactor);
		}

		union {
			struct{ float x, y, z, w; };
			float m[4];
		};

	};

	struct MAT4
	{
		MAT4() : MAT4(1.0f) {};
		MAT4(const MAT4& mat) { memcpy(m, mat.m, sizeof(float)*16); }

		MAT4(const float I)
		{
			SetRow(0, { I, 0, 0, 0 });
			SetRow(1, { 0, I, 0, 0 });
			SetRow(2, { 0, 0, I, 0 });
			SetRow(3, { 0, 0, 0, I });
		}

		inline void SetRow(UINT row, const float(&newRows)[4])
		{
			if (row < 4)
			{
				for (size_t i = 0; i < 4; ++i)
				{
					m[row][i] = newRows[i];
				}
			}
		}

		inline VEC4 GetRow(UINT row) const
		{
			return VEC4(m[row][0], m[row][1], m[row][2], m[row][3]);
		}

		inline void SetColumn(UINT col, const float(&newCols)[4])
		{
			if (col < 4)
			{
				for (size_t i = 0; i < 4; i++)
				{
					m[i][col] = newCols[i];
				}
			}
		}

		inline VEC4 GetColumn(UINT col) const
		{
			return VEC4(m[0][col], m[1][col], m[2][col], m[3][col]);
		}

		VEC4 operator*(const VEC4& vec4) const
		{
			VEC4 retVec;
			retVec.x = m[0][0] * vec4.x + m[0][1] * vec4.y +
				+m[0][2] * vec4.z + m[0][3] * vec4.w;
			retVec.y = m[1][0] * vec4.x + m[1][1] * vec4.y +
				+m[1][2] * vec4.z + m[1][3] * vec4.w;
			retVec.z = m[2][0] * vec4.x + m[2][1] * vec4.y +
				+m[2][2] * vec4.z + m[2][3] * vec4.w;
			retVec.w = m[3][0] * vec4.x + m[3][1] * vec4.y +
				+m[3][2] * vec4.z + m[3][3] * vec4.w;

			return retVec;
		}

		friend MAT4 operator*(const VEC4& vec4, const MAT4& mat4)
		{
			MAT4 retMat;
			for (size_t i = 0; i < 4; ++i)
			{
				for (size_t j = 0; j < 4; ++j)
				{
					VEC4 row_i = mat4.m[i];
					VEC4 column_j = { mat4.m[0][j], mat4.m[1][j], mat4.m[2][j], mat4.m[3][j] };
					retMat.m[i][j] = row_i * column_j;
				}
			}

			return retMat;
		}

		MAT4 operator*(const MAT4& mat4) const
		{
			MAT4 retMat;
			// Right matrix cols.
			auto col1 = this->operator*(mat4.GetColumn(0));
			auto col2 = this->operator*(mat4.GetColumn(1));
			auto col3 = this->operator*(mat4.GetColumn(2));
			auto col4 = this->operator*(mat4.GetColumn(3));

			// Left matrix rows.
			auto row1 = GetRow(0);
			auto row2 = GetRow(1);
			auto row3 = GetRow(2);
			auto row4 = GetRow(3);

			retMat.SetRow(0, { row1*col1, row1*col2, row1*col3, row1*col4 });
			retMat.SetRow(1, { row2*col1, row2*col2, row2*col3, row2*col4 });
			retMat.SetRow(2, { row3*col1, row3*col2, row3*col3, row3*col4 });
			retMat.SetRow(3, { row4*col1, row4*col2, row4*col3, row4*col4 });

			return retMat;
		}

		MAT4 T() const
		{
			MAT4 retMat;
			for (size_t i = 0; i < 4; i++)
			{
				for (size_t j = 0; j < 4; j++)
				{
					retMat.m[i][j] = m[j][i];
				}

			}

			return retMat;
		}

		union {
			struct { float _11, _12, _13, _14, _21, _22, _23, _24, _31, _32, _33, _34, _41, _42, _43, _44; };
			float m[4][4];
		};

	};

	inline float Clamp(float val, float min, float max) { if (val > max)return max; else if (val < min) return min; else return val; };
	inline float Clamp(float val, UINT min, UINT max) { return Clamp(val, float(min), float(max)); };

	inline float Lerp(const float start, const float end, const float fraction) { return start + (end - start)*fraction; };
	inline VEC2 Lerp(const VEC2& start, const VEC2& end, float fraction) { return VEC2(Lerp(start.x, end.x, fraction), Lerp(start.y, end.y, fraction)); };

	inline VEC3 Reflect(const VEC3& direction, const VEC3& normal)
	{
		// Input can be unNormalized vectors.
		auto I = direction.Normalize();
		auto N = normal.Normalize();
		auto S = -I.DotProduct(N) * N;
		auto P = I + S;
		auto R = 2 * P - I;

		return R;
	};

	inline void ClampColor(VEC3& color)
	{
		color.x = Clamp(color.x, 0.f, 1.0f);
		color.y = Clamp(color.y, 0.f, 1.0f);
		color.z = Clamp(color.z, 0.f, 1.0f);
	}

	inline void MakePerspectiveMatrix_DX(MAT4& mat, const float viewRadianY, const float aspectRatio, const float nearPlane, const float farPlane)
	{
		// Get a perspective matrix. return Z is LINEAR.
		mat.SetRow(0, { 1.0f / (aspectRatio * tan(viewRadianY)), 0, 0, 0 });// Aspect Ratio : x/y.
		mat.SetRow(1, { 0, 1.0f / tan(viewRadianY), 0, 0 });
		mat.SetRow(2, { 0, 0, farPlane / (farPlane - nearPlane), -nearPlane * farPlane / (farPlane - nearPlane) });
		mat.SetRow(3, { 0, 0, 1, 0 });
	}

	inline void MakePerspectiveMatrix_GL(MAT4& mat, const float viewRadianY, const float aspectRatio, const float nearPlane, const float farPlane)
	{
		mat.SetRow(0, { 1.0f / (aspectRatio * tan(viewRadianY)), 0, 0, 0 });// Aspect Ratio : x/y.
		mat.SetRow(1, { 0, 1.0f / tan(viewRadianY), 0, 0 });
		mat.SetRow(2, { 0, 0, -1.f * (farPlane + nearPlane) / (farPlane - nearPlane), -2.f * nearPlane * farPlane / (farPlane - nearPlane) });
		mat.SetRow(3, { 0, 0, -1.f, 0 });
	}

	inline MAT4 RemoveTranslation(const MAT4& mat)
	{
		MAT4 retMat(mat);

		for (size_t i = 0; i < 4; i++)
		{
			retMat.m[i][3] = retMat.m[3][i] = 0.f;
		}

		return retMat;
	}

	/////////////////////////////////////////////////////////////
	// Matrices for rotate.

	inline MAT4 Matrix_RotationXYZ(float x, float y, float z)// Input radians.
	{
		MAT4 outMatrix;

		outMatrix.SetRow(0, { cos(y)*cos(z),
			sin(x)*sin(y)*cos(z) - cos(x)*sin(z), sin(x)*sin(z) + cos(x)*sin(y)*cos(z), 0 });
		outMatrix.SetRow(1, { cos(y)*sin(z), sin(x)*sin(y)*sin(z) + cos(x)*cos(z), cos(x)*sin(y)*sin(z) - sin(x)*cos(z), 0 });
		outMatrix.SetRow(2, { -sin(y), sin(x)*cos(y), cos(x)*cos(y), 0 });
		outMatrix.SetRow(3, { 0, 0, 0, 1 });

		return outMatrix;
	};


	inline float GetRadian(float angle) { return angle * PI / 180.0f; };
}

#endif // !JKMATH_H_
