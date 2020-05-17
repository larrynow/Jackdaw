#pragma once
#include"Vector2.h"

namespace jkMath
{
	struct VEC3
	{
		// Zero vecor.
		VEC3() : x(0), y(0), z(0) {}
		VEC3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
		VEC3(const float column[3])
		{
			x = column[0];
			y = column[1];
			z = column[2];
		}
		explicit VEC3(float _v) : VEC3(_v, _v, _v) {}
		VEC3(const VEC2& _vec2, float _z) : VEC3(_vec2.x, _vec2.y, _z) {}
		VEC3(const VEC3& _vec) = default;

		inline const float(&GetValue() const)[3]
		{
			return m;
		}

		inline float SquareLength() const
		{
			return x * x + y * y + z*z;
		}
		inline float Length() const
		{
			return sqrt(SquareLength());
		}
		inline VEC3 Normalize() const
		{
			float length = Length();
			if (length == 0)
			{
				return VEC3();
			}
			else
			{
				return VEC3(x / length, y / length, z / length);
			}
		}

		inline bool operator==(const VEC3& _vec) const
		{
			return (x == _vec.x && y == _vec.y && z == _vec.z);
		}
		inline bool operator!=(const VEC3& _vec) const
		{
			return !operator==(_vec);
		}

		inline const VEC3 operator+(const VEC3& _vec) const
		{
			return VEC3(x + _vec.x, y + _vec.y, z + _vec.z);
		}
		inline const VEC3& operator+=(const VEC3& _vec)
		{
			x += _vec.x;
			y += _vec.y;
			z += _vec.z;

			return *this;
		}

		inline const VEC3 operator-(const VEC3& _vec) const
		{
			return VEC3(x - _vec.x, y - _vec.y, z - _vec.z);
		}
		inline const VEC3& operator-=(const VEC3& _vec)
		{
			x -= _vec.x;
			y -= _vec.y;
			z -= _vec.z;

			return *this;
		}
		inline const VEC3 operator-()const
		{
			return VEC3(-x, -y, -z);
		}

		inline const VEC3 operator*(const float scaleFactor) const
		{
			return VEC3(x * scaleFactor, y * scaleFactor, z * scaleFactor);
		}
		inline friend const VEC3 operator*(const float scaleFactor, const VEC3 _vec)
		{
			return _vec * scaleFactor;
		}
		inline const float operator*(const VEC3& _vec) const//scalar product
		{
			return x * _vec.x + y * _vec.y + z * _vec.z;
		}
		inline const VEC3& operator*=(const float scaleFactor)
		{
			x *= scaleFactor;
			y *= scaleFactor;
			z *= scaleFactor;

			return *this;
		}

		inline const VEC3 operator/(const float scaleFactor) const
		{
			return VEC3(x / scaleFactor, y / scaleFactor, z / scaleFactor);
		}
		inline const VEC3& operator/=(const float scaleFactor)
		{
			x /= scaleFactor;
			y /= scaleFactor;
			z /= scaleFactor;

			return *this;
		}

		inline float operator[](size_t i) const
		{
			if (i == 0) return x;
			else if (i == 1) return y;
			else if (i == 2) return z;
			else return 0.f;
		}

		inline VEC3 CrossProduct(const VEC3& _vec) const
		{
			return VEC3(_vec.z * y - _vec.y * z, _vec.x * z - _vec.z * x, _vec.y * x - _vec.x * y);
		}
		inline float DotProduct(const VEC3& _vec) const
		{
			return _vec.x * x + _vec.y * y + _vec.z * z;
		}

		inline float CosineValue(const VEC3& _vec) const
		{
			return (*this).Normalize().DotProduct(_vec.Normalize());
		}

		inline friend std::ostream& operator<<(std::ostream& os, const VEC3 _vec)
		{
			return os << _vec.x << "," << _vec.y << "," << _vec.z << std::endl;
		}

		union {
			struct {
				float x, y, z;
			};
			float m[3];
		};
	};
}