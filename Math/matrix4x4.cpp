#include "stdafx.h"

#include "matrix4x4.h"
#include <cassert>
#include <iostream>
#include "common.h"
#include "vector3f.h"


const Matrix4x4 Matrix4x4::ZERO(0.0f, 0.0f, 0.0f, 0.0f,
								0.0f, 0.0f, 0.0f, 0.0f,
								0.0f, 0.0f, 0.0f, 0.0f,
								0.0f, 0.0f, 0.0f, 0.0f);

const Matrix4x4 Matrix4x4::IDENTITY(1.0f, 0.0f, 0.0f, 0.0f,
									0.0f, 1.0f, 0.0f, 0.0f,
									0.0f, 0.0f, 1.0f, 0.0f,
									0.0f, 0.0f, 0.0f, 1.0f);

Matrix4x4::Matrix4x4()
{
	memset(m, 0, sizeof(float) * 16);
	m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.0f;
}


Matrix4x4::Matrix4x4(float m11, float m12, float m13, float m14,
					float m21, float m22, float m23, float m24,
					float m31, float m32, float m33, float m34,
					float m41, float m42, float m43, float m44)
{
	m[0][0] = m11;	m[0][1] = m12;	m[0][2] = m13;	m[0][3] = m14;
	m[1][0] = m21;	m[1][1] = m22;	m[1][2] = m23;	m[1][3] = m24;
	m[2][0] = m31;	m[2][1] = m32;	m[2][2] = m33;	m[2][3] = m34;
	m[3][0] = m41;	m[3][1] = m42;	m[3][2] = m43;	m[3][3] = m44;
}

Matrix4x4::Matrix4x4(float afMat[16])
{
	memcpy(m, afMat, sizeof(float) * 16);
}

Matrix4x4::Matrix4x4(const Matrix4x4& mat)
{
	memcpy(m, mat.m, sizeof(float) * 16);
}

float& Matrix4x4::operator()(size_t row, size_t col)
{
	assert(row < 4 && col < 4);
	return m[row][col];
}

float Matrix4x4::operator()(size_t row, size_t col) const
{
	assert(row < 4 && col < 4);
	return m[row][col];
}

Matrix4x4& Matrix4x4::operator=(const Matrix4x4& rhs)
{
	memcpy(m, rhs.m, sizeof(float) * 16);
	return *this;
}

Matrix4x4& Matrix4x4::operator+=(const Matrix4x4& rhs)
{
	for ( int i = 0; i < 4; ++i )
		for ( int j = 0; j < 4; ++j )
			m[i][j] += rhs.m[i][j];
	return *this;
}

Matrix4x4& Matrix4x4::operator-=(const Matrix4x4& rhs)
{
	for ( int i = 0; i < 4; ++i )
		for ( int j = 0; j < 4; ++j )
			m[i][j] -= rhs.m[i][j];
	return *this;
}

Matrix4x4& Matrix4x4::operator*=(float fScalar)
{
	for ( int i = 0; i < 4; ++i )
		for ( int j = 0; j < 4; ++j )
			m[i][j] *= fScalar;
	return *this;
}

Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& rhs)
{
	Matrix4x4 temp;
	for ( int i = 0; i < 4; ++i )
	{
		for ( int j = 0; j < 4; ++j )
		{
			temp(i, j) = 0.0f;
			for ( int k = 0; k < 4; ++k )
				temp.m[i][j] += m[i][k] * rhs.m[k][j];
		}
	}
	memcpy(m, temp.m, sizeof(float) * 16);
	return *this;
}

Matrix4x4& Matrix4x4::operator/=(float fScalar)
{
	float recip = 1 / fScalar;
	for ( int i = 0; i < 4; ++i )
		for ( int j = 0; j < 4; ++j )
			m[i][j] *= recip;
	return *this;
}

Matrix4x4 Matrix4x4::operator + () const
{
	return *this;
}

Matrix4x4 Matrix4x4::operator - () const
{
	Matrix4x4 ret(*this);
	ret *= -1.0f;
	return ret;
}

void Matrix4x4::SetToIdentity()
{
	memset(m, 0, sizeof(float) * 16);
	m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.0f;
}

Matrix4x4 Matrix4x4::Transpose()
{
	return Matrix4x4(m[0][0], m[1][0], m[2][0], m[3][0],
					m[0][1], m[1][1], m[2][1], m[3][1],
					m[0][2], m[1][2], m[2][2], m[3][2],
					m[0][3], m[1][3], m[2][3], m[3][3]);
}

// Óà×ÓÊ½
inline static float MINOR(const Matrix4x4& m, const size_t r0, const size_t r1, const size_t r2, 
	const size_t c0, const size_t c1, const size_t c2)
{
	return m[r0][c0] * (m[r1][c1] * m[r2][c2] - m[r2][c1] * m[r1][c2]) -
		m[r0][c1] * (m[r1][c0] * m[r2][c2] - m[r2][c0] * m[r1][c2]) +
		m[r0][c2] * (m[r1][c0] * m[r2][c1] - m[r2][c0] * m[r1][c1]);
}

float Matrix4x4::Determinant() const
{
	return m[0][0] * MINOR(*this, 1, 2, 3, 1, 2, 3) -
		m[0][1] * MINOR(*this, 1, 2, 3, 0, 2, 3) +
		m[0][2] * MINOR(*this, 1, 2, 3, 0, 1, 3) -
		m[0][3] * MINOR(*this, 1, 2, 3, 0, 1, 2);
}

Matrix4x4 Matrix4x4::Adjoint() const
{
	return Matrix4x4( MINOR(*this, 1, 2, 3, 1, 2, 3),
		-MINOR(*this, 0, 2, 3, 1, 2, 3),
		MINOR(*this, 0, 1, 3, 1, 2, 3),
		-MINOR(*this, 0, 1, 2, 1, 2, 3),

		-MINOR(*this, 1, 2, 3, 0, 2, 3),
		MINOR(*this, 0, 2, 3, 0, 2, 3),
		-MINOR(*this, 0, 1, 3, 0, 2, 3),
		MINOR(*this, 0, 1, 2, 0, 2, 3),

		MINOR(*this, 1, 2, 3, 0, 1, 3),
		-MINOR(*this, 0, 2, 3, 0, 1, 3),
		MINOR(*this, 0, 1, 3, 0, 1, 3),
		-MINOR(*this, 0, 1, 2, 0, 1, 3),

		-MINOR(*this, 1, 2, 3, 0, 1, 2),
		MINOR(*this, 0, 2, 3, 0, 1, 2),
		-MINOR(*this, 0, 1, 3, 0, 1, 2),
		MINOR(*this, 0, 1, 2, 0, 1, 2));
}

 
Matrix4x4 Matrix4x4::Inverse()
{
	float m00 = m[0][0], m01 = m[0][1], m02 = m[0][2], m03 = m[0][3];
	float m10 = m[1][0], m11 = m[1][1], m12 = m[1][2], m13 = m[1][3];
	float m20 = m[2][0], m21 = m[2][1], m22 = m[2][2], m23 = m[2][3];
	float m30 = m[3][0], m31 = m[3][1], m32 = m[3][2], m33 = m[3][3];

	float v0 = m20 * m31 - m21 * m30;
	float v1 = m20 * m32 - m22 * m30;
	float v2 = m20 * m33 - m23 * m30;
	float v3 = m21 * m32 - m22 * m31;
	float v4 = m21 * m33 - m23 * m31;
	float v5 = m22 * m33 - m23 * m32;

	float t00 = + (v5 * m11 - v4 * m12 + v3 * m13);
	float t10 = - (v5 * m10 - v2 * m12 + v1 * m13);
	float t20 = + (v4 * m10 - v2 * m11 + v0 * m13);
	float t30 = - (v3 * m10 - v1 * m11 + v0 * m12);

	float invDet = 1 / (t00 * m00 + t10 * m01 + t20 * m02 + t30 * m03);

	float d00 = t00 * invDet;
	float d10 = t10 * invDet;
	float d20 = t20 * invDet;
	float d30 = t30 * invDet;

	float d01 = - (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
	float d11 = + (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
	float d21 = - (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
	float d31 = + (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

	v0 = m10 * m31 - m11 * m30;
	v1 = m10 * m32 - m12 * m30;
	v2 = m10 * m33 - m13 * m30;
	v3 = m11 * m32 - m12 * m31;
	v4 = m11 * m33 - m13 * m31;
	v5 = m12 * m33 - m13 * m32;

	float d02 = + (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
	float d12 = - (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
	float d22 = + (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
	float d32 = - (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

	v0 = m21 * m10 - m20 * m11;
	v1 = m22 * m10 - m20 * m12;
	v2 = m23 * m10 - m20 * m13;
	v3 = m22 * m11 - m21 * m12;
	v4 = m23 * m11 - m21 * m13;
	v5 = m23 * m12 - m22 * m13;

	float d03 = - (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
	float d13 = + (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
	float d23 = - (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
	float d33 = + (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

	return Matrix4x4(
		d00, d01, d02, d03,
		d10, d11, d12, d13,
		d20, d21, d22, d23,
		d30, d31, d32, d33);
}

//----------------------------------------------------------------------------------------
Matrix4x4 operator+(const Matrix4x4& lhs, const Matrix4x4& rhs)
{
	Matrix4x4 ret(lhs);
	ret += rhs;
	return ret;
}

Matrix4x4 operator-(const Matrix4x4& lhs, const Matrix4x4& rhs)
{
	Matrix4x4 ret(lhs);
	ret -= rhs;
	return ret;
}

Matrix4x4 operator*(const Matrix4x4& lhs, const Matrix4x4& rhs)
{
	Matrix4x4 ret;
	for ( int i = 0; i < 4; ++i )
	{
		for ( int j = 0; j < 4; ++j )
		{
			ret.m[i][j] = 0.0f;
			for ( int k = 0; k < 4; ++k )
			{
				ret.m[i][j] += lhs.m[i][k] * rhs.m[k][j];
			}
		}
	}
	return ret;
}

Matrix4x4 operator*(const Matrix4x4& mat, float t)
{
	Matrix4x4 ret(mat);
	ret *= t;
	return ret;
}

Matrix4x4 operator*(float t, const Matrix4x4& mat)
{
	Matrix4x4 ret(mat);
	ret *= t;
	return ret;
}

Vector4f operator*( const Vector4f& vec, const Matrix4x4& mat)
{
	Vector4f ret(0, 0, 0, 0);

	for ( int j = 0; j < 4; ++j )
		for ( int i = 0; i < 4; ++i )
			ret[j] += vec[i] * mat.m[i][j];

	return ret;	
}

Matrix4x4 operator/(const Matrix4x4& mat, float t)
{
	float recip = 1 / t;
	Matrix4x4 ret(mat);
	ret *= recip;
	return ret;
}
//----------------------------------------------------------------------------------------
Matrix4x4 Translate(float tx, float ty, float tz)
{
	Matrix4x4 ret;

	ret.m[3][0] = tx;
	ret.m[3][1] = ty;
	ret.m[3][2] = tz;

	return ret;
}

Matrix4x4 Translate(const Vector3f& tv)
{
	return Translate(tv.x, tv.y, tv.z);
}

Matrix4x4 RotateX(float radian)
{
	Matrix4x4 ret;

	float cosPhi = std::cos(radian);
	float sinPhi = std::sin(radian);

	ret.m[1][1] =  cosPhi; ret.m[1][2] = sinPhi;
	ret.m[2][1] = -sinPhi; ret.m[2][2] = cosPhi;

	return ret;
}

Matrix4x4 RotateY(float radian)
{
	Matrix4x4 ret;

	float cosPhi = std::cos(radian);
	float sinPhi = std::sin(radian);

	ret.m[0][0] = cosPhi; ret.m[0][2] = -sinPhi;
	ret.m[2][0] = sinPhi; ret.m[2][2] = cosPhi;

	return ret;
}

Matrix4x4 RotateZ(float radian)
{
	Matrix4x4 ret;

	float cosPhi = std::cos(radian);
	float sinPhi = std::sin(radian);

	ret.m[0][0] =  cosPhi; ret.m[0][1] = sinPhi;
	ret.m[1][0] = -sinPhi; ret.m[1][1] = cosPhi;

	return ret;
}

Matrix4x4 Scale(float sx, float sy, float sz)
{
	Matrix4x4 ret;

	ret.m[0][0] = sx;
	ret.m[1][1] = sy;
	ret.m[2][2] = sz;

	return ret;
}

Matrix4x4 Scale(float s)
{
	return Scale(s, s, s);
}

// depth range [0, 1]
Matrix4x4 Ortho(float l, float r, float b, float t, float n, float f)
{
	Matrix4x4 ret;

	float rml = 1 / (r - l); // rml stands for "r minus l", for convinience, we pre-compute its reciprocal
	float tmb = 1 / (t - b); 
	float fmn = 1 / (f - n);

	ret.m[0][0] = 2 * rml;
	ret.m[1][1] = 2 * tmb;
	ret.m[2][2] = fmn;

	ret.m[3][0] = -(r + l) * rml;
	ret.m[3][1] = -(t + b) * tmb;
	ret.m[3][2] = -n * fmn;

	return ret;
}

Matrix4x4 Frustum(float l, float r, float b, float t, float n, float f)
{
	Matrix4x4 ret;

	float rml = 1 / (r - l);
	float tmb = 1 / (t - b); 
	float fmn = 1 / (f - n);

	ret.m[0][0] = 2 * n * rml;
	ret.m[1][1] = 2 * n * tmb;
	ret.m[2][2] = f * fmn;
	ret.m[3][3] = 0;

	ret.m[2][0] = -(r + l) * rml;	 
	ret.m[2][1] = -(t + b) * tmb;

	ret.m[2][3] = 1;		
	ret.m[3][2] = -f * n * fmn;

	return ret;
}

Matrix4x4 Perspective(float fovy, float aspectRatio, float n, float f)
{
	float scale = tan(fovy * 0.5f) * n;
	float r = aspectRatio * scale, l = -r;
	float t = scale, b = -t;
	return Frustum(l, r, b, t, n, f);
}

Matrix4x4 LookAt(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz)
{
	return LookAt(Vector3f(eyex, eyey, eyez), Vector3f(centerx, centery, centerz), Vector3f(upx, upy, upz));
}

Matrix4x4 LookAt(const Vector3f& eye, const Vector3f& center, const Vector3f& up)
{
	Matrix4x4 ret;
	// uvn camera
	Vector3f v = up;
	Vector3f n = center - eye; n.Normalize();
	Vector3f u = CrossProduct(v, n); u.Normalize();
	v = CrossProduct(n, u);

	ret.m[0][0] = u.x; ret.m[0][1] = v.x; ret.m[0][2] = n.x; ret.m[0][3] = 0;
	ret.m[1][0] = u.y; ret.m[1][1] = v.y; ret.m[1][2] = n.y; ret.m[1][3] = 0;
	ret.m[2][0] = u.z; ret.m[2][1] = v.z; ret.m[2][2] = n.z; ret.m[2][3] = 0;
	ret.m[3][0] = -DotProduct(u, eye); 
	ret.m[3][1] = -DotProduct(v, eye); 
	ret.m[3][2] = -DotProduct(n, eye); 
	ret.m[3][3] = 1;

	return ret;
}

Matrix4x4 ViewPort(UINT32 x, UINT32 y, UINT32 width, UINT32 height, UINT16 minz, UINT16 maxz)
{
	Matrix4x4 ret;

	float halfw = float(width  >> 1);
	float halfh = float(height >> 1);
	ret.m[0][0] =  halfw; 
	ret.m[1][1] = -halfh;
	ret.m[2][2] = float(maxz - minz);
	ret.m[3][0] = float(x + halfw);
	ret.m[3][1] = float(y + halfh);
	ret.m[3][2] = float(minz);

	return ret;
}

//----------------------------------------------------------------------------------------
bool operator==(const Matrix4x4& lhs, const Matrix4x4& rhs)
{
	for (int i = 0; i < 4; ++i)
		for ( int j = 0; j < 4; ++j )
			if ( lhs.m[i][j] != rhs.m[i][j] )
				return false;

	return true;
}

bool operator!=(const Matrix4x4& lhs, const Matrix4x4& rhs)
{
	return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& os, const Matrix4x4& mat)
{
	for ( int i = 0; i < 4; ++i )
	{
		for ( int j = 0 ; j < 4; ++j )
		{
			os << mat.m[i][j]<< "\t";
		}
		os << std::endl;
	}
	return os;
}
