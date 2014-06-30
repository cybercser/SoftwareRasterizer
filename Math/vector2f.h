#ifndef VECTOR2F_H
#define VECTOR2F_H

#include <iostream>

class Vector2f
{
public:
	float x;
	float y;

public:
	Vector2f();
	Vector2f(const Vector2f& rhs);
	Vector2f(float fX, float fY);
	explicit Vector2f(float afCoord[2]);

	inline float& operator [] (const size_t i);

	inline float operator [] (const size_t i) const;

	inline Vector2f operator +();
	inline Vector2f operator -();

	// assignment operators
	inline Vector2f& operator = (const Vector2f& rhs);
	inline Vector2f& operator += (const Vector2f& rhs);
	inline Vector2f& operator -= (const Vector2f& rhs);
	inline Vector2f& operator *= (float fScalar);
	inline Vector2f& operator /= (float fScalar);

	// geometric methods
	inline float Length() const;

	inline float SquaredLength() const;

	inline void Normalize();
};

/************************************************************************/
/* Arithmetic operators                                                 */
/************************************************************************/
Vector2f operator + (const Vector2f& lhs, const Vector2f& rhs);
Vector2f operator - (const Vector2f& lhs, const Vector2f& rhs);
Vector2f operator * (const Vector2f& lhs, float fScalar);
Vector2f operator * (float fScalar, const Vector2f& rhs);
Vector2f operator / (const Vector2f& lhs, float fScalar);

/************************************************************************/
/* Equality operators                                                   */
/************************************************************************/
bool operator == (const Vector2f& lhs, const Vector2f& rhs);
bool operator != (const Vector2f& lhs, const Vector2f& rhs);

/************************************************************************/
/* Stream inserter                                                      */
/************************************************************************/
std::ostream& operator << (std::ostream& os, const Vector2f& vec);

/************************************************************************/
/* Geometric methods                                                    */
/************************************************************************/
float DotProduct(const Vector2f& v1, const Vector2f& v2);

#endif // VECTOR2F_H