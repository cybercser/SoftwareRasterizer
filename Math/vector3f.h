#ifndef VECTOR3F_H
#define VECTOR3F_H

#include <iostream>

class Vector3f
{
public:
	float x, y, z;

	static const Vector3f ZERO;
	static const Vector3f UNIT_X;
	static const Vector3f UNIT_Y;
	static const Vector3f UNIT_Z;
	static const Vector3f NEGATIVE_UNIT_X;
	static const Vector3f NEGATIVE_UNIT_Y;
	static const Vector3f NEGATIVE_UNIT_Z;

public:
	Vector3f() {}
	Vector3f(const Vector3f& v);
	Vector3f(float fX, float fY, float fZ);
	explicit Vector3f(float afCoord[3]);

	float& operator [] (const size_t i);

	float operator [] (const size_t i) const;

	Vector3f operator +() const;
	Vector3f operator -() const;

	// assignment operators
	Vector3f& operator = (const Vector3f& rhs);
	Vector3f& operator += (const Vector3f& rhs);
	Vector3f& operator -= (const Vector3f& rhs);
	Vector3f& operator *= (float fScalar);
	Vector3f& operator /= (float fScalar);

	// geometric methods
	float Length() const;
	float SquaredLength() const;
	void Normalize();
};

/************************************************************************/
/* Arithmetic operators                                                 */
/************************************************************************/
Vector3f operator + (const Vector3f& lhs, const Vector3f& rhs);
Vector3f operator - (const Vector3f& lhs, const Vector3f& rhs);
Vector3f operator * (const Vector3f& lhs, float fScalar);
Vector3f operator * (float fScalar, const Vector3f& rhs);
Vector3f operator / (const Vector3f& lhs, float fScalar);

/************************************************************************/
/* Equality operators                                                   */
/************************************************************************/
bool operator == (const Vector3f& lhs, const Vector3f& rhs);
bool operator != (const Vector3f& lhs, const Vector3f& rhs);

/************************************************************************/
/* Stream inserter                                                      */
/************************************************************************/
std::ostream& operator << (std::ostream& os, const Vector3f& vec);

/************************************************************************/
/* Geometric methods                                                    */
/************************************************************************/
float DotProduct(const Vector3f& v1, const Vector3f& v2);
Vector3f CrossProduct(const Vector3f& v1, const Vector3f& v2);

#endif // VECTOR3F_H