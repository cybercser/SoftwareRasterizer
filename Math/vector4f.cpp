#include "stdafx.h"
#include <cassert>
#include <iostream>
#include "common.h"
#include "vector4f.h"

Vector4f::Vector4f()
{
}

Vector4f::Vector4f(const Vector4f& rhs)
	: x(rhs.x),	y(rhs.y), z(rhs.z), w(rhs.w)
{
}

Vector4f::Vector4f(float fX, float fY, float fZ, float fW)
	: x(fX), y(fY), z(fZ), w(fW)
{
}

Vector4f::Vector4f(const Vector3f& v, float fW)
	: x(v.x), y(v.y), z(v.z), w(fW)
{
}

Vector4f::Vector4f(float afCoord[4])
	: x(afCoord[0]), y(afCoord[1]),	z(afCoord[2]), w(afCoord[3])
{
}

float& Vector4f::operator [] (const size_t i)
{
	assert (i < 4);

	return *(&x+i);
}

float Vector4f::operator [] (const size_t i) const
{
	assert(i < 4);

	return *(&x+i);
}

Vector4f Vector4f::operator+()
{
	return *this;
}

Vector4f Vector4f::operator -()
{
	Vector4f ret(-x, -y, -z, -w);
	return ret;
}

// assignment operators
Vector4f&  Vector4f::operator = (const Vector4f& rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	w = rhs.w;

	return *this;
}

Vector4f&  Vector4f::operator += (const Vector4f& rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	w += rhs.w;

	return *this;
}

Vector4f&  Vector4f::operator -= (const Vector4f& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	w -= rhs.w;

	return *this;
}

Vector4f&  Vector4f::operator *= (float fScalar)
{
	x *= fScalar;
	y *= fScalar;
	z *= fScalar;
	w *= fScalar;

	return *this;
}

Vector4f&  Vector4f::operator /= (float fScalar)
{
	assert(fScalar != 0.0);

	float fInv = 1.0f / fScalar;

	x *= fInv;
	y *= fInv;
	z *= fInv;
	w *= fInv;

	return *this;
}

// geometric methods
float  Vector4f::GetLength() const
{
	return std::sqrt(x * x + y * y + z * z + w * w);
}

float  Vector4f::GetSquaredLength() const
{
	return (x * x + y * y + z * z + w * w);
}

void  Vector4f::Normalize()
{
	float fLength = GetLength();

	if (FloatEqual(fLength, 0.0f) || FloatEqual(fLength, 1.0f))
	{
		return;
	}
	else
	{
		float fInvLength = 1.0f / fLength;
		x *= fInvLength;
		y *= fInvLength;
		z *= fInvLength;
		w *= fInvLength;
	}
}

Vector3f  Vector4f::ToVector3f() const
{
	return Vector3f(x, y, z);
}

/************************************************************************/
/* Arithmetic operators                                                 */
/************************************************************************/
Vector4f operator + (const Vector4f& lhs, const Vector4f& rhs)
{
	Vector4f ret(lhs);
	ret += rhs;

	return ret;
}

Vector4f operator - (const Vector4f& lhs, const Vector4f& rhs)
{
	Vector4f ret(lhs);
	ret -= rhs;

	return ret;
}

Vector4f operator * (const Vector4f& lhs, float fScalar)
{
	Vector4f ret(lhs);
	ret *= fScalar;

	return ret;
}

Vector4f operator*( float fScalar, const Vector4f& rhs )
{
	Vector4f ret(rhs);
	ret *= fScalar;

	return ret;
}

Vector4f operator / (const Vector4f& lhs, float fScalar)
{
	Vector4f ret(lhs);
	ret /= fScalar;

	return ret;
}

/************************************************************************/
/* Equality operators                                                   */
/************************************************************************/
bool operator == (const Vector4f& lhs, const Vector4f& rhs)
{
	return ((lhs.x == rhs.x) &&
		(lhs.y == rhs.y) &&
		(lhs.z == rhs.z) &&
		(lhs.w == rhs.w));
}

bool operator != (const Vector4f& lhs, const Vector4f& rhs)
{
	return !(lhs == rhs);
}

/************************************************************************/
/* Stream inserter                                                      */
/************************************************************************/
std::ostream& operator << (std::ostream& os, const Vector4f& v)
{
	os << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
	return os;
}

Vector4f LERP(const Vector4f& v1, const Vector4f& v2, float t)
{
	return Vector4f(LERPf(v1.x, v2.x, t), 
		LERPf(v1.y, v2.y, t), 
		LERPf(v1.z, v2.z, t), 
		LERPf(v1.w, v2.w, t));
}
