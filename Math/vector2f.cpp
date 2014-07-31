#include "stdafx.h"
#include <cassert>
#include "common.h"
#include "vector2f.h"


Vector2f::Vector2f()
{
}

Vector2f::Vector2f( const Vector2f& rhs ) : x(rhs.x),
	y(rhs.y)
{
}

Vector2f::Vector2f( float fX, float fY ) : x(fX), y(fY)
{
}

Vector2f::Vector2f( float afCoord[2] ) : x(afCoord[0]),
	y(afCoord[1])
{
}

float& Vector2f::operator [] (const size_t i)
{
	assert (i < 2);
	return *(&x+i);
}

float Vector2f::operator [] (const size_t i) const
{
	assert(i < 2);
	return *(&x+i);
}

Vector2f Vector2f::operator +()
{
	return *this;
}

Vector2f Vector2f::operator -()
{
	Vector2f ret(-x, -y);
	return ret;
}

// assignment operators
Vector2f& Vector2f::operator = (const Vector2f& rhs)
{
	x = rhs.x;
	y = rhs.y;

	return *this;
}

Vector2f& Vector2f::operator += (const Vector2f& rhs)
{
	x += rhs.x;
	y += rhs.y;

	return *this;
}

Vector2f& Vector2f::operator -= (const Vector2f& rhs)
{
	x -= rhs.x;
	y -= rhs.y;

	return *this;
}

Vector2f& Vector2f::operator *= (float fScalar)
{
	x *= fScalar;
	y *= fScalar;

	return *this;
}

Vector2f& Vector2f::operator /= (float fScalar)
{
	assert(fScalar != 0.0);

	float fInv = 1.0f / fScalar;

	x *= fInv;
	y *= fInv;

	return *this;
}

// geometric methods
float Vector2f::Length() const
{
	return std::sqrt(x * x + y * y);
}

float Vector2f::SquaredLength() const
{
	return (x * x + y * y);
}

void Vector2f::Normalize()
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
	}
}

/************************************************************************/
/* Arithmetic operators                                                 */
/************************************************************************/
Vector2f operator + (const Vector2f& lhs, const Vector2f& rhs)
{
	Vector2f ret(lhs);
	ret += rhs;

	return ret;
}

Vector2f operator - (const Vector2f& lhs, const Vector2f& rhs)
{
	Vector2f ret(lhs);
	ret -= rhs;

	return ret;
}

Vector2f operator * (const Vector2f& lhs, float fScalar)
{
	Vector2f ret(lhs);
	ret *= fScalar;

	return ret;
}

Vector2f operator*( float fScalar, const Vector2f& rhs )
{
	Vector2f ret(rhs);
	ret *= fScalar;

	return ret;
}

Vector2f operator / (const Vector2f& lhs, float fScalar)
{
	Vector2f ret(lhs);
	ret /= fScalar;

	return ret;
}

/************************************************************************/
/* Equality operators                                                   */
/************************************************************************/
bool operator == (const Vector2f& lhs, const Vector2f& rhs)
{
	return ( (lhs.x == rhs.x) && (lhs.y == rhs.y) );
}

bool operator != (const Vector2f& lhs, const Vector2f& rhs)
{
	return !(lhs == rhs);
}

/************************************************************************/
/* Stream inserter                                                      */
/************************************************************************/
std::ostream& operator << (std::ostream& os, const Vector2f& vec)
{
	os << "(" << vec.x << ", " << vec.y << ")";
	return os;
}

/************************************************************************/
/* Geometric methods                                                    */
/************************************************************************/
float DotProduct(const Vector2f& v1, const Vector2f& v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}
