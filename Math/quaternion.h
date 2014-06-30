#ifndef QUATERNION_H
#define QUATERNION_H

#include <iostream>
#include "vector4f.h"

class Quaternion
{
public:
	float w;
	float x;
	float y;
	float z;

public:
	Quaternion();
	Quaternion(const Quaternion& q);
	Quaternion(const float fW, const float fX, const float fY, const float fZ);
	Quaternion(const Vector4f& v);
	// Quaternion(const float yaw, const float pitch, const float roll);

	// assignment operators
	inline Quaternion& operator = (const Quaternion& q);
	inline Quaternion& operator += (const Quaternion& q);
	inline Quaternion& operator -= (const Quaternion& q);
	inline Quaternion& operator *= (const Quaternion& q);
	inline Quaternion& operator *= (const float t);
	inline Quaternion& operator /= (const float t);

	// geometric methods
	inline Quaternion Conjugate() const;
	inline float Length() const;
	inline float SquaredLength() const;
	inline void Normalize();
	inline Quaternion Inverse() const;

	// TODO 
	// to_matrix4x4
	// 
};

/************************************************************************/
/* Arithmetic operators                                                 */
/************************************************************************/
Quaternion operator + (const Quaternion& lhs, const Quaternion& rhs);
Quaternion operator - (const Quaternion& lhs, const Quaternion& rhs);
Quaternion operator * (const Quaternion& lhs, const Quaternion& rhs);
Quaternion operator * (const Quaternion& lhs, const float t);
Quaternion operator * (const float t, const Quaternion& rhs);

/************************************************************************/
/* Euqality operators                                                   */
/************************************************************************/
bool operator == (const Quaternion& lhs, const Quaternion& rhs);
bool operator != (const Quaternion& lhs, const Quaternion& rhs);

/************************************************************************/
/* Stream inserter                                                      */
/************************************************************************/
std::ostream& operator << (std::ostream& os, const Quaternion& q);

/************************************************************************/
/* Geometric methods                                                    */
/************************************************************************/
float DotProduct(const Quaternion& q1, const Quaternion& q2);

#endif // QUATERNION_H 