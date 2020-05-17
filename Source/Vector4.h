#pragma once
#include"Vector3.h"

namespace jkMath
{
	struct VEC4
	{
		VEC4() :x(0), y(0), z(0), w(0) {}
		VEC4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
		VEC4(const float column[4])
		{
			x = column[0];
			y = column[1];
			z = column[2];
			w = column[3];
		}
		explicit VEC4(float _v) : x(_v), y(_v), z(_v), w(_v) {}
		VEC4(const VEC3& _vec3, float _w) : x(_vec3.x), y(_vec3.y), z(_vec3.z), w(_w) {}
		VEC4(const VEC4& _vec) = default;

		inline const float(&GetValue() const)[4]
		{
			return m;
		}
		inline VEC3 xyz() const
		{
			return VEC3(x, y, z);
		}

		inline float SquareLength() const
		{
			return x * x + y * y + z * z + w * w;
		}
		inline float Length() const
		{
			return sqrt(SquareLength());
		}
		inline VEC4 Normalize()
		{
			float length = Length();
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

		inline bool operator==(const VEC4& _vec) const
		{
			return (x == _vec.x && y == _vec.y && z == _vec.z && w == _vec.w);
		}
		inline bool operator!=(const VEC4& _vec) const
		{
			return !operator==(_vec);
		}

		inline const VEC4 operator+(const VEC4& _vec) const
		{
			return VEC4(x + _vec.x, y + _vec.y, z + _vec.z, w + _vec.w);
		}
		inline const VEC4& operator+=(const VEC4& _vec)
		{
			x += _vec.x;
			y += _vec.y;
			z += _vec.z;
			w += _vec.w;

			return *this;
		}

		inline const VEC4 operator-(const VEC4& _vec) const
		{
			return VEC4(x - _vec.x, y - _vec.y, z - _vec.z, w - _vec.w);
		}
		inline const VEC4& operator-=(const VEC4& _vec)
		{
			x -= _vec.x;
			y -= _vec.y;
			z -= _vec.z;
			w -= _vec.w;

			return *this;
		}
		inline const VEC4 operator-()const
		{
			return VEC4(-x, -y, -z, -w);
		}

		inline const VEC4 operator*(const float scaleFactor) const
		{
			return VEC4(x * scaleFactor, y * scaleFactor, z * scaleFactor, w * scaleFactor);
		}
		inline friend const VEC4 operator*(const float scaleFactor, VEC4 _vec)
		{
			return _vec * scaleFactor;
		}
		inline const float operator*(const VEC4 _vec) const//scalar product
		{
			return x * _vec.x + y * _vec.y + z * _vec.z + w * _vec.w;
		}
		inline const VEC4& operator*=(const float scaleFactor)
		{
			x *= scaleFactor;
			y *= scaleFactor;
			z *= scaleFactor;
			w *= scaleFactor;

			return *this;
		}

		inline const VEC4 operator/(const float scaleFactor) const
		{
			return VEC4(x / scaleFactor, y / scaleFactor, z / scaleFactor, w / scaleFactor);
		}
		inline const VEC4& operator/=(const float scaleFactor)
		{
			x /= scaleFactor;
			y /= scaleFactor;
			z /= scaleFactor;
			w /= scaleFactor;

			return *this;
		}

		inline float operator[](size_t i) const
		{
			if (i == 0) return x;
			else if (i == 1) return y;
			else if (i == 2) return z;
			else if (i == 3) return w;
			else return 0.f;
		}

		inline float DotProduct(const VEC4& _vec) const
		{
			return _vec.x * x + _vec.y * y + _vec.z * z + _vec.w * w;
		}

		inline friend std::ostream& operator<<(std::ostream& os, const VEC4 _vec)
		{
			return os << _vec.x << "," << _vec.y << "," << _vec.z << "," << _vec.w << std::endl;
		}

		// In order x, y, z, w.
		union {
			struct {
				float x, y, z, w;
			};
			float m[4];
		};

	};
}