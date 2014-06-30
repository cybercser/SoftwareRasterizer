#ifndef VECTOR4F_H
#define VECTOR4F_H

#include "vector3f.h"

class  Vector4f
{
public:
	float x, y, z, w;

public:
	Vector4f();
	Vector4f(const Vector4f& rhs);
	Vector4f(float fX, float fY, float fZ, float fW = 1.0f);
	Vector4f(const Vector3f& v, float fW);
	explicit Vector4f(float afCoord[4]);

	float& operator [] (const size_t i);

	float operator [] (const size_t i) const;
	
	Vector4f operator +();
	Vector4f operator -();

	// assignment operators
	Vector4f& operator = (const Vector4f& rhs);
	Vector4f& operator += (const Vector4f& rhs);
	Vector4f& operator -= (const Vector4f& rhs);
	Vector4f& operator *= (float fScalar);
	Vector4f& operator /= (float fScalar);

	// geometric methods
	float GetLength() const;

	float GetSquaredLength() const;

	void Normalize();

	Vector3f ToVector3f() const;
};
/************************************************************************/
/* Arithmetic operators                                                 */
/************************************************************************/
Vector4f operator + (const Vector4f& lhs, const Vector4f& rhs);
Vector4f operator - (const Vector4f& lhs, const Vector4f& rhs);
Vector4f operator * (const Vector4f& lhs, float fScalar);
Vector4f operator * (float fScalar, const Vector4f& rhs);
Vector4f operator / (const Vector4f& lhs, float fScalar);

/************************************************************************/
/* Equality operators                                                   */
/************************************************************************/
bool operator == (const Vector4f& lhs, const Vector4f& rhs);
bool operator != (const Vector4f& lhs, const Vector4f& rhs);

/************************************************************************/
/* Stream inserter                                                      */
/************************************************************************/
std::ostream& operator << (std::ostream& os, const Vector4f& v);

Vector4f LERP(const Vector4f& v1, const Vector4f& v2, float t);
#endif // VECTOR4F_H