#include "stdafx.h"
#include <cassert>
#include "common.h"
#include "vector3f.h"

Vector3f::Vector3f(const Vector3f& v)
	: x(v.x),
	y(v.y),
	z(v.z)
{
}

Vector3f::Vector3f(float fX, float fY, float fZ)
	: x(fX), y(fY), z(fZ)
{
}

Vector3f::Vector3f(float afCoord[3])
	: x(afCoord[0]),
	y(afCoord[1]),
	z(afCoord[2])
{
}

float& Vector3f::operator [] (const size_t i)
{
	assert (i < 3);

	return *(&x+i);
}

float Vector3f::operator [] (const size_t i) const
{
	assert(i < 3);

	return *(&x+i);
}

Vector3f Vector3f::operator + () const
{
	return *this;
}

Vector3f Vector3f::operator - () const
{
	Vector3f ret(-x, -y, -z);
	return ret;
}

// assignment operators
Vector3f& Vector3f::operator = (const Vector3f& rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;

	return *this;
}

Vector3f& Vector3f::operator += (const Vector3f& rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;

	return *this;
}

Vector3f& Vector3f::operator -= (const Vector3f& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;

	return *this;
}

Vector3f& Vector3f::operator *= (float fScalar)
{
	x *= fScalar;
	y *= fScalar;
	z *= fScalar;

	return *this;
}

Vector3f& Vector3f::operator /= (float fScalar)
{
	assert(fScalar != 0.0);

	float fInv = 1.0f / fScalar;

	x *= fInv;
	y *= fInv;
	z *= fInv;

	return *this;
}

// geometric methods
float Vector3f::Length() const
{
	return std::sqrt(x * x + y * y + z * z);
}

float Vector3f::SquaredLength() const
{
	return (x * x + y * y + z * z);
}

void Vector3f::Normalize()
{
	float fLength = Length();

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
	}
}

/************************************************************************/
/* Arithmetic operators                                                 */
/************************************************************************/
Vector3f operator + (const Vector3f& lhs, const Vector3f& rhs)
{
	Vector3f ret(lhs);
	ret += rhs;

	return ret;
}

Vector3f operator - (const Vector3f& lhs, const Vector3f& rhs)
{
	Vector3f ret(lhs);
	ret -= rhs;

	return ret;
}

Vector3f operator * (const Vector3f& lhs, float fScalar)
{
	Vector3f ret(lhs);
	ret *= fScalar;

	return ret;
}

Vector3f operator*( float fScalar, const Vector3f& rhs )
{
	Vector3f ret(rhs);
	ret *= fScalar;

	return ret;
}

Vector3f operator / (const Vector3f& lhs, float fScalar)
{
	Vector3f ret(lhs);
	ret /= fScalar;

	return ret;
}

/************************************************************************/
/* Equality operators                                                   */
/************************************************************************/
bool operator == (const Vector3f& lhs, const Vector3f& rhs)
{
	return ( (lhs.x == rhs.x) &&
		(lhs.y == rhs.y) &&
		(lhs.z == rhs.z) );
}
bool operator != (const Vector3f& lhs, const Vector3f& rhs)
{
	return !(lhs == rhs);
}

/************************************************************************/
/* Stream inserter                                                      */
/************************************************************************/
std::ostream& operator << (std::ostream& os, const Vector3f& vec)
{
	os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
	return os;
}

/************************************************************************/
/* Geometric methods                                                    */
/************************************************************************/
float DotProduct(const Vector3f& v1, const Vector3f& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vector3f CrossProduct(const Vector3f& v1, const Vector3f& v2)
{
	Vector3f ret;
	ret.x = v1.y * v2.z - v1.z * v2.y;
	ret.y = v1.z * v2.x - v1.x * v2.z;
	ret.z = v1.x * v2.y - v1.y * v2.x;

	return ret;
}

const Vector3f Vector3f::ZERO				= Vector3f( 0.0f,  0.0f,  0.0f);
const Vector3f Vector3f::UNIT_X				= Vector3f( 1.0f,  0.0f,  0.0f);
const Vector3f Vector3f::UNIT_Y				= Vector3f( 0.0f,  1.0f,  0.0f);
const Vector3f Vector3f::UNIT_Z				= Vector3f( 0.0f,  0.0f,  1.0f);
const Vector3f Vector3f::NEGATIVE_UNIT_X	= Vector3f(-1.0f,  0.0f,  0.0f);
const Vector3f Vector3f::NEGATIVE_UNIT_Y	= Vector3f( 0.0f, -1.0f,  0.0f);
const Vector3f Vector3f::NEGATIVE_UNIT_Z	= Vector3f( 0.0f,  0.0f, -1.0f);