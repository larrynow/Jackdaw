#pragma once
#include<math.h>
#include<iostream>

namespace jkMath
{
	struct VEC2
	{
		VEC2() : x(0), y(0) {}
		VEC2(float _x, float _y) : x(_x), y(_y) {}
		explicit VEC2(float _v) : x(_v), y(_v) {}
		VEC2(const VEC2& _vec) = default;

		inline const float(&GetValue() const)[2]
		{
			return m;
		}

		inline float SquareLength() const
		{
			return x * x + y * y;
		}
		inline float Length() const
		{
			return sqrt(SquareLength());
		}
		inline VEC2 Normalize() const
		{
			float length = Length();
			if (length == 0)
			{
				return VEC2();
			}
			else
			{
				return VEC2(x / length, y / length);
			}
		}

		inline bool operator==(const VEC2& _vec) const
		{
			return (x == _vec.x && y == _vec.y);
		}
		inline bool operator!=(const VEC2& _vec) const
		{
			return !operator==(_vec);
		}

		inline const VEC2 operator+(const VEC2& _vec) const
		{
			return VEC2(x + _vec.x, y + _vec.y);
		}
		inline const VEC2& operator += (const VEC2& _vec)
		{
			x += _vec.x;
			y += _vec.y;

			return *this;
		}

		inline const VEC2 operator-(const VEC2& _vec) const
		{
			return VEC2(x - _vec.x, y - _vec.y);
		}
		inline const VEC2& operator -= (const VEC2& _vec)
		{
			x -= _vec.x;
			y -= _vec.y;

			return *this;
		}
		inline const VEC2 operator-()const
		{
			return VEC2(-x, -y);
		}

		inline const VEC2 operator*(const float scaleFactor) const
		{
			return VEC2(x * scaleFactor, y * scaleFactor);
		}
		inline friend const VEC2 operator*(const float scaleFactor, VEC2 _vec)
		{
			return _vec * scaleFactor;
		}
		inline const float operator*(const VEC2& _vec) const//scalar product
		{
			return x * _vec.x + y * _vec.y;
		}
		inline const VEC2& operator *= (const float _f)
		{
			x *= _f;
			y *= _f;

			return *this;
		}

		inline const VEC2 operator/(const float scaleFactor) const
		{
			return VEC2(x / scaleFactor, y / scaleFactor);
		}
		inline const VEC2& operator /= (const float _f)
		{
			x /= _f;
			y /= _f;

			return *this;
		}

		inline float operator[](unsigned int i) const
		{
			if (i == 0) return x;
			else if (i == 1) return y;
			else return 0.f;
		}

		inline float DotProduct(const VEC2& _vec) const
		{
			return x * _vec.x + y * _vec.y;
		}

		inline friend std::ostream& operator<<(std::ostream& os, const VEC2 _vec)
		{
			return os << _vec.x << "," << _vec.y << std::endl;
		}

		union {
			struct {
				float x, y;
			};
			float m[2];
		};
	};
}