#ifndef QUATERNION_H
#define QUATERNION_H

#include "common.h"
#include <iostream>

// Forward declaration
class Vector3f;
class Matrix3x3;

class Quaternion
{
public:
    float w;
    float x;
    float y;
    float z;

    static const Quaternion IDENTITY;

public:
    Quaternion();
    Quaternion(const Quaternion& q);
    Quaternion(float fW, float fX, float fY, float fZ);
    Quaternion(const Vector3f& axis, float radian);

    /// Array accessor operator
    float operator [] ( const size_t i ) const;
    /// Array accessor operator
    float& operator [] ( const size_t i );

    // Unary operators
    Quaternion operator- () const;
    Quaternion operator+ () const;

    // assignment operators
    inline Quaternion& operator = (const Quaternion& q);
    inline Quaternion& operator += (const Quaternion& q);
    inline Quaternion& operator -= (const Quaternion& q);
    inline Quaternion& operator *= (const Quaternion& q);
    inline Quaternion& operator *= (float t);
    inline Quaternion& operator /= (float t);

    // Utilities
    Quaternion Conjugate() const;
    float Norm() const;
    void Normalize();
    Quaternion Inverse() const;
    //     Quaternion Exp() const;
    //     Quaternion Log() const;

    void ToRotationMatrix(Matrix3x3& kRot) const;
    void ToAxisAngle(Vector3f& rkAxis, float& rfAngle) const;

    static Quaternion FromAxisAngle(float x, float y, float z, float angle);
    static Quaternion FromAxisAngle(const Vector3f& axis, float radian);
    static Quaternion FromRotationMatrix (const Matrix3x3& kRot);
    static Quaternion Nlerp(const Quaternion & q1, const Quaternion & q2, float t);
    static Quaternion Slerp(const Quaternion & q1, const Quaternion & q2, float t);
    static Quaternion Squad(const Quaternion& q0, const Quaternion& a0, const Quaternion& a1, const Quaternion& q1, float t);
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