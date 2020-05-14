#pragma once
#ifndef JKMATH_H_
#define JKMATH_H_

#include<math.h>
#include<iostream>

typedef unsigned int UINT;
typedef unsigned long long U64;

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
		// Zero vecor.
		VEC3() : x(0), y(0), z(0) {};
		VEC3(const VEC3& _vec) :x(_vec.x), y(_vec.y), z(_vec.z) {};
		VEC3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {};
		VEC3(const float column[3])
		{
			x = column[0];
			y = column[1];
			z = column[2];
		}
		explicit VEC3(float _v) : VEC3(_v, _v, _v) {};
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
			//struct { float r, g, b; };
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

		float DotProduct(const VEC4& _vec) const
		{
			return _vec.x * x + _vec.y * y + _vec.z * z + _vec.w * w;
		}

		union {
			struct{ float x, y, z, w; };
			struct{ float r, g, b, a; };
			float m[4];
		};

	};

	typedef VEC4 Quaternion;

	struct MAT4
	{
		//Default as identity matrix.
		MAT4() : MAT4(1.0f) {};
		MAT4(const MAT4& mat) { memcpy(m, mat.m, sizeof(float)*16); }

		MAT4(const float I)
		{
			SetRow(0, { I, 0, 0, 0 });
			SetRow(1, { 0, I, 0, 0 });
			SetRow(2, { 0, 0, I, 0 });
			SetRow(3, { 0, 0, 0, I });
		}

		inline float* operator[](std::size_t i)
		{
			return m[i];
		}

		inline const float GetValue(int i, int j) const
		{
			return m[i][j];
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

		inline void SetColumn(UINT col, const VEC4& newCol)
		{
			if (col < 4)
			{
				for (size_t i = 0; i < 4; i++)
				{
					m[i][col] = newCol.m[i];
				}
			}
		}

		inline VEC4 GetColumn(UINT col) const
		{
			return VEC4(m[0][col], m[1][col], m[2][col], m[3][col]);
		}

		const VEC4 operator*(const VEC4& vec4) const
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

		friend const MAT4 operator*(const VEC4& vec4, const MAT4& mat4)
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

		const MAT4 operator*(const MAT4& mat4) const
		{
			MAT4 retMat;
			// Right matrix cols.
			//auto col1 = this->operator*(mat4.GetColumn(0));
			//auto col2 = this->operator*(mat4.GetColumn(1));
			//auto col3 = this->operator*(mat4.GetColumn(2));
			//auto col4 = this->operator*(mat4.GetColumn(3));

			//retMat.SetColumn(0, col1);
			//retMat.SetColumn(1, col2);
			//retMat.SetColumn(2, col3);
			//retMat.SetColumn(3, col4);


			// Left matrix rows.
			auto row1 = GetRow(0);
			auto row2 = GetRow(1);
			auto row3 = GetRow(2);
			auto row4 = GetRow(3);

			auto col1 = mat4.GetColumn(0);
			auto col2 = mat4.GetColumn(1);
			auto col3 = mat4.GetColumn(2);
			auto col4 = mat4.GetColumn(3);

			retMat.SetRow(0, { row1*col1, row1*col2, row1*col3, row1*col4 });
			retMat.SetRow(1, { row2*col1, row2*col2, row2*col3, row2*col4 });
			retMat.SetRow(2, { row3*col1, row3*col2, row3*col3, row3*col4 });
			retMat.SetRow(3, { row4*col1, row4*col2, row4*col3, row4*col4 });

			return retMat;
		}

		const MAT4 T() const
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

		/*MAT4 Inv() const
		{
			MAT4 retMat;

		}*/

		union {
			struct { float _11, _12, _13, _14, _21, _22, _23, _24, _31, _32, _33, _34, _41, _42, _43, _44; };
			float m[4][4];
		};

	};

	template<typename T>
	inline T Clamp(const T& val, const T& min, const T& max){ if (val > max)return max; else if (val < min) return min; else return val; }
	
	template<typename T>
	inline float Fraction(const T left, const T right, const T value) { return right==left?0:(value - left) / (right - left); }

	template<typename T>
	inline T Lerp(const T& start, const T& end, const float fraction) { return start + (end - start) * fraction; };
	

	inline Quaternion QuaternionSlerp(const Quaternion& q1, const Quaternion& q2, const float fraction)
	{
		if (fraction == 0.f) return q1;
		else if (fraction == 0.f) return q2;
		if (q1 == q2) return q1;

		float halfY, alpha, beta;
		float u, f1, f2a, f2b;
		float ratio1, ratio2;
		float halfSecHalfTheta, versHalfTheta;
		float sqNotU, sqU;

		float cosTheta = q1.DotProduct(q2);

		// As usual in all slerp implementations, we fold theta.
		alpha = cosTheta >= 0 ? 1.0f : -1.0f;
		halfY = 1.0f + alpha * cosTheta;

		// Here we bisect the interval, so we need to fold t as well.
		f2b = fraction - 0.5f;
		u = f2b >= 0 ? f2b : -f2b;
		f2a = u - f2b;
		f2b += u;
		u += u;
		f1 = 1.0f - u;

		// One iteration of Newton to get 1-cos(theta / 2) to good accuracy.
		halfSecHalfTheta = 1.09f - (0.476537f - 0.0903321f * halfY) * halfY;
		halfSecHalfTheta *= 1.5f - halfY * halfSecHalfTheta * halfSecHalfTheta;
		versHalfTheta = 1.0f - halfY * halfSecHalfTheta;

		// Evaluate series expansions of the coefficients.
		sqNotU = f1 * f1;
		ratio2 = 0.0000440917108f * versHalfTheta;
		ratio1 = -0.00158730159f + (sqNotU - 16.0f) * ratio2;
		ratio1 = 0.0333333333f + ratio1 * (sqNotU - 9.0f) * versHalfTheta;
		ratio1 = -0.333333333f + ratio1 * (sqNotU - 4.0f) * versHalfTheta;
		ratio1 = 1.0f + ratio1 * (sqNotU - 1.0f) * versHalfTheta;

		sqU = u * u;
		ratio2 = -0.00158730159f + (sqU - 16.0f) * ratio2;
		ratio2 = 0.0333333333f + ratio2 * (sqU - 9.0f) * versHalfTheta;
		ratio2 = -0.333333333f + ratio2 * (sqU - 4.0f) * versHalfTheta;
		ratio2 = 1.0f + ratio2 * (sqU - 1.0f) * versHalfTheta;

		// Perform the bisection and resolve the folding done earlier.
		f1 *= ratio1 * halfSecHalfTheta;
		f2a *= ratio2;
		f2b *= ratio2;
		alpha *= f1 + f2a;
		beta = f1 + f2b;

		// Apply final coefficients to a and b as usual.
		float w = alpha * q1.m[0] + beta * q2.m[0];
		float x = alpha * q1.m[1] + beta * q2.m[1];
		float y = alpha * q1.m[2] + beta * q2.m[2];
		float z = alpha * q1.m[3] + beta * q2.m[3];

		// This final adjustment to the quaternion's length corrects for
		// any small constraint error in the inputs q1 and q2 But as you
		// can see, it comes at the cost of 9 additional multiplication
		// operations. If this error-correcting feature is not required,
		// the following code may be removed.
		f1 = 1.5f - 0.5f * (w * w + x * x + y * y + z * z);
		return Quaternion(w, x, y, z) * f1;
	}

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

	inline void MakeLookAtMatrix(MAT4& mat, const VEC3& eye, 
		const VEC3& lookingDirection, const VEC3& up)
	{
		// First a shift matrix.
		MAT4 shiftMat;
		shiftMat.SetRow(0, { 1, 0, 0, -eye.x });
		shiftMat.SetRow(1, { 0, 1, 0, -eye.y });
		shiftMat.SetRow(2, { 0, 0, 1, -eye.z });
		shiftMat.SetRow(3, { 0, 0, 0, 1 });

		// Then made a vector space tans matrix.
		// Camera space in left-hand coordinates.
		VEC3 frontVector = lookingDirection.Normalize();
		VEC3 rightVector = frontVector.CrossProduct(up.Normalize());
		VEC3 upVector = rightVector.CrossProduct(frontVector);
		MAT4 transMat;
		transMat.SetRow(0, { rightVector.x, rightVector.y, rightVector.z, 0 });
		transMat.SetRow(1, { upVector.x, upVector.y, upVector.z, 0 });
		//transMat.SetRow(1, { 0.f, 1.f, 0.f, 0 });
		transMat.SetRow(2, { -frontVector.x, -frontVector.y, -frontVector.z, 0 });
		transMat.SetRow(3, { 0, 0, 0, 1 });

		mat = transMat * shiftMat;
	}

	inline MAT4 GetLookAtMatrix(const VEC3& eye, const VEC3& lookingDirection,
		const VEC3& upVector)
	{
		MAT4 lookAtMatrix;
		MakeLookAtMatrix(lookAtMatrix, eye, lookingDirection, upVector);

		return lookAtMatrix;
	}

	inline void MakeOrthMatrix(MAT4& mat, float left, float right, float bottom, float top, float near, float far)
	{
		mat.SetRow(0, { 2 / (right - left), 0, 0, -(left + right) / (right - left) });
		mat.SetRow(1, { 0, 2 / (top - bottom), 0, (bottom + top) / (bottom - top) });
		mat.SetRow(2, { 0, 0, -2 / (far - near), (near + far) / (near - far) });
		mat.SetRow(3, { 0, 0, 0, 1});
	}

	inline void MakeTranslateMatrix(MAT4& mat, float x, float y, float z)
	{
		mat.m[0][3] = x;
		mat.m[1][3] = y;
		mat.m[2][3] = z;
	}

	inline void MakeTranslateMatrix(MAT4& mat, const VEC3& translateVec)
	{
		MakeTranslateMatrix(mat, translateVec.x, translateVec.y, translateVec.z);
	}

	inline void MakeScaleMatrix(MAT4& mat, 
		const float xScale, const float yScale, const float zScale)
	{
		mat.m[0][0] = xScale;
		mat.m[1][1] = yScale;
		mat.m[2][2] = zScale;
	}

	inline void MakeScaleMatrix(MAT4& mat, const VEC3& scaleVec)
	{
		MakeScaleMatrix(mat, scaleVec.x, scaleVec.y, scaleVec.z);
	}

	inline void MakeScaleMatrix(MAT4& mat, float scale)
	{
		MakeScaleMatrix(mat, scale, scale, scale);
	}

	//Remove translation part of a matrix.
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

	//Rotate order : x-y-z(outter).
	inline void MakeRotationMatrix_Euler(MAT4& ret, const VEC3& eulerAngle)// Input radians.
	{
		const float& x = eulerAngle.x;
		const float& y = eulerAngle.y;
		const float& z = eulerAngle.z;

		ret.SetRow(0, { cos(y) * cos(z),
			sin(x) * sin(y) * cos(z) - cos(x) * sin(z), sin(x) * sin(z) + cos(x) * sin(y) * cos(z), 0 });
		ret.SetRow(1, { cos(y) * sin(z), 
			sin(x) * sin(y) * sin(z) + cos(x) * cos(z), cos(x) * sin(y) * sin(z) - sin(x) * cos(z), 0 });
		ret.SetRow(2, { -sin(y), sin(x) * cos(y), cos(x) * cos(y), 0 });
		ret.SetRow(3, { 0, 0, 0, 1 });
	}
	inline void MakeRotationMatrix_Euler(MAT4& ret, 
		const float pitch, const float yaw, const float roll)
	{
		MakeRotationMatrix_Euler(ret, { pitch, yaw, roll });
	}

	inline MAT4 GetRotationMatrix_Euler(float x, float y, float z)// Input radians.
	{
		MAT4 outMatrix;
		MakeRotationMatrix_Euler(outMatrix, { x, y, z });

		return outMatrix;
	}

	inline void MakeRotationMatrix_Quaternion(MAT4& ret, const VEC4& quaternion)
	{
		auto& w = quaternion.m[0];
		auto& x = quaternion.m[1];
		auto& y = quaternion.m[2];
		auto& z = quaternion.m[3];

		ret.m[0][0] = 1.f - 2.f* (y* y + z * z);
		ret.m[0][1] = 2.f* (x* y - z * w);
		ret.m[0][2] = 2.f* (x* z + y * w);
		ret.m[1][0] = 2.f* (x* y + z * w);
		ret.m[1][1] = 1.f - 2.f* (x* x + z * z);
		ret.m[1][2] = 2.f* (y* z - x * w);
		ret.m[2][0] = 2.f* (x* z - y * w);
		ret.m[2][1] = 2.f* (y* z + x * w);
		ret.m[2][2] = 1.f - 2.f* (x* x + y * y);
	}

	inline float GetRadian(float angle) { return angle * PI / 180.0f; };
	inline VEC3 GetRadian(const VEC3& angle) 
	{ 
		return VEC3(GetRadian(angle.x), GetRadian(angle.y), GetRadian(angle.z)); 
	}
}

#endif // !JKMATH_H_
