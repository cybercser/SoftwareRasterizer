#include "stdafx.h"

#include "quaternion.h"

#include "vector3f.h"
#include "matrix3x3.h"

#include <cassert>

const Quaternion Quaternion::IDENTITY(1.0f, 0.0f, 0.0f, 0.0f);

Quaternion::Quaternion()
    : w(1), x(0), y(0), z(0)
{
}

Quaternion::Quaternion(const Quaternion& q)
    : w(q.w), x(q.x), y(q.y), z(q.z)
{
}

Quaternion::Quaternion(float fW, float fX, float fY, float fZ)
    : w(fW), x(fX), y(fY), z(fZ)
{
}

Quaternion::Quaternion(const Vector3f& axis, float radian)
{
    // adapted from Geometric Tools code http://www.geometrictools.com/LibMathematics/Algebra/Wm5Quaternion.inl
    // assert: axis is unit length
    //
    // The quaternion representing the roatation is
    // q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

    float halfAngle = 0.5 * radian;
    float sn = sin(halfAngle);
    w = cos(halfAngle);
    x = sn * axis.x;
    y = sn * axis.y;
    z = sn * axis.z;
}

float Quaternion::operator [] ( const size_t i ) const
{
    assert( i < 4 );

    return *(&w+i);
}

/// Array accessor operator
float& Quaternion::operator [] ( const size_t i )
{
    assert( i < 4 );

    return *(&w+i);
}

Quaternion Quaternion::operator-() const
{
    return Quaternion(-w, -x, -y, -z);
}

Quaternion Quaternion::operator+() const
{
    return *this;
}

Quaternion& Quaternion::operator=(const Quaternion& q)
{
    w = q.w;
    x = q.x;
    y = q.y;
    z = q.z;
    return *this;
}

Quaternion& Quaternion::operator+=(const Quaternion& q)
{
    w += q.w;
    x += q.x;
    y += q.y;
    z += q.z;
    return *this;
}

Quaternion& Quaternion::operator-=(const Quaternion& q)
{
    w -= q.w;
    x -= q.x;
    y -= q.y;
    z -= q.z;
    return *this;
}

Quaternion& Quaternion::operator*=(const Quaternion& q)
{
    return Quaternion(w*q.w - x*q.x - y*q.y - z*q.z,
        y*q.z - z*q.y + x*q.w + w*q.x,
        z*q.x - x*q.z + y*q.w + w*q.y,
        x*q.y - y*q.x + z*q.w + w*q.z);
}

Quaternion& Quaternion::operator*=(float t)
{
    w *= t;
    x *= t;
    y *= t;
    z *= t;
    return *this;
}

Quaternion& Quaternion::operator/=(float t)
{
    t = 1 / t;
    w *= t;
    x *= t;
    y *= t;
    z *= t;
    return *this;
}

Quaternion Quaternion::Conjugate() const
{
    return Quaternion(w, -x, -y, -z);
}

float Quaternion::Norm() const
{
    return sqrtf(w*w + x*x + y*y + z*z);
}

void Quaternion::Normalize()
{
    float t = 1 / Norm();
    w *= t;
    x *= t;
    y *= t;
    z *= t;
}

Quaternion Quaternion::Inverse() const
{
    float sNorm = 1 / (w*w + x*x + y*y + z*z);
    return Quaternion(w*sNorm, -x*sNorm, -y*sNorm, -z*sNorm);
}

void Quaternion::ToRotationMatrix(Matrix3x3& rot) const
{
    // http://www.flipcode.com/documents/matrfaq.html#Q38
    float twoX  = 2*x;
    float twoY  = 2*y;
    float twoZ  = 2*z;
    float twoWX = twoX*w;
    float twoWY = twoY*w;
    float twoWZ = twoZ*w;
    float twoXX = twoX*x;
    float twoXY = twoY*x;
    float twoXZ = twoZ*x;
    float twoYY = twoY*y;
    float twoYZ = twoZ*y;
    float twoZZ = twoZ*z;

    rot(0,0) = (float)1 - (twoYY + twoZZ);
    rot(0,1) = twoXY + twoWZ;
    rot(0,2) = twoXZ - twoWY;
    rot(1,0) = twoXY - twoWZ;
    rot(1,1) = (float)1 - (twoXX + twoZZ);
    rot(1,2) = twoYZ + twoWX;
    rot(2,0) = twoXZ + twoWY;    
    rot(2,1) = twoYZ - twoWX;    
    rot(2,2) = (float)1 - (twoXX + twoYY);
}

void Quaternion::ToAxisAngle(Vector3f& axis, float& angle) const
{
    // The quaternion representing the rotation is
    //   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

    float sqrLength = x*x + y*y + z*z;

    if (sqrLength > FLOAT_EPSILON)
    {
        angle = 2*acos(x);
        float invLength = 1/sqrt(sqrLength);
        axis.x = x*invLength;
        axis.y = y*invLength;
        axis.z = z*invLength;
    }
    else
    {
        // Angle is 0 (mod 2*pi), so any axis will do.
        angle = 0;
        axis.x = 1;
        axis.y = 0;
        axis.z = 0;
    }
}

Quaternion Quaternion::FromAxisAngle(float x, float y, float z, float radian)
{
    return Quaternion(Vector3f(x, y, z), radian);
}

Quaternion Quaternion::FromAxisAngle(const Vector3f& axis, float radian)
{
    return Quaternion(axis, radian);
}

Quaternion Quaternion::FromRotationMatrix(const Matrix3x3& rot)
{
    // http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/
    // adapted from Geometric Tools code http://www.geometrictools.com/LibMathematics/Algebra/Wm5Quaternion.inl

    // Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
    // article "Quaternion Calculus and Fast Animation".
    Quaternion q;
    const int next[3] = { 1, 2, 0 };

    float trace = rot(0, 0) + rot(1, 1) + rot(2, 2);
    float root;

    if (trace > 0)
    {
        // |w| > 1/2, may as well choose w > 1/2
        root = sqrtf(trace + 1);    // 2w
        q.w = 0.5 * root;
        root = 0.5/root;
        q.x = (rot(1,2) - rot(2,1))*root;
        q.y = (rot(2,0) - rot(0,2))*root;
        q.z = (rot(0,1) - rot(1,0))*root;
    } 
    else
    {
        // |w| <= 1/2
        int i = 0;
        if (rot(1,1) > rot(0,0))
        {
            i = 1;
        }
        if (rot(2,2) > rot(i,i))
        {
            i = 2;
        }
        int j = next[i];
        int k = next[j];

        root = sqrtf(rot(i,i) - rot(j,j) - rot(k,k) + 1);
        float* quat[3] = { &q.x, &q.y, &q.z };
        *quat[i] = 0.5*root;
        q.w = (rot(j,k) - rot(k,j))*root;
        *quat[j] = (rot(i,j) + rot(j,i))*root;
        *quat[k] = (rot(i,k) + rot(k,i))*root;
    }

    return q;
}

Quaternion Quaternion::Nlerp(const Quaternion & q1, const Quaternion & q2, float t)
{
    return q1*(1-t) + q2*t;
}

Quaternion Quaternion::Slerp(const Quaternion & q1, const Quaternion & q2, float t)
{
    // spherical interpolation
    // q = q_1 * sin((1-t)Theta) / sin(Theta) + q_2 * sin(tTheta) / sin(Theta)
    float cs = DotProduct(q1, q2);
    float angle = acosf(cs);
    Quaternion q(q1);

    if (fabs(angle) >= FLOAT_EPSILON)
    {
        float sn = sin(angle);
        float invSn = 1/sn;
        float tAngle = t*angle;
        float coeff0 = sin(angle - tAngle)*invSn;
        float coeff1 = sin(tAngle)*invSn;

        q = q1 * coeff0 + q2 * coeff1;
    } 

    return q;
}

Quaternion Quaternion::Squad(const Quaternion& q0, const Quaternion& a0, const Quaternion& a1, const Quaternion& q1, float t)
{
    //http://3dgep.com/?p=1815#SQUAD
    float slerpT = 2*t*(1-t);
    Quaternion slerpP = Slerp(q0, q1, t);
    Quaternion slerpQ = Slerp(a0, a1, t);
    return Slerp(slerpP, slerpQ, slerpT);
}

// Quaternion Quaternion::Exp() const
// {
//     // If q = A*(x*i+y*j+z*k) where (x,y,z) is unit length, then
//     // exp(q) = cos(A)+sin(A)*(x*i+y*j+z*k).  If sin(A) is near zero,
//     // use exp(q) = cos(A)+A*(x*i+y*j+z*k) since A/sin(A) has limit 1.
// 
//     Quaternion result;
// 
//     Real angle = Math<Real>::Sqrt(mTuple[1]*mTuple[1] +
//         mTuple[2]*mTuple[2] + mTuple[3]*mTuple[3]);
// 
//     Real sn = Math<Real>::Sin(angle);
//     result.mTuple[0] = Math<Real>::Cos(angle);
// 
//     int i;
// 
//     if (Math<Real>::FAbs(sn) >= Math<Real>::ZERO_TOLERANCE)
//     {
//         Real coeff = sn/angle;
//         for (i = 1; i < 4; ++i)
//         {
//             result.mTuple[i] = coeff*mTuple[i];
//         }
//     }
//     else
//     {
//         for (i = 1; i < 4; ++i)
//         {
//             result.mTuple[i] = mTuple[i];
//         }
//     }
// 
//     return result;
// }
// 
// Quaternion Quaternion::Log() const
// {
//     // If q = cos(A)+sin(A)*(x*i+y*j+z*k) where (x,y,z) is unit length, then
//     // log(q) = A*(x*i+y*j+z*k).  If sin(A) is near zero, use log(q) =
//     // sin(A)*(x*i+y*j+z*k) since sin(A)/A has limit 1.
// 
//     Quaternion result;
//     result.mTuple[0] = (Real)0;
// 
//     int i;
// 
//     if (Math<Real>::FAbs(mTuple[0]) < (Real)1)
//     {
//         Real angle = Math<Real>::ACos(mTuple[0]);
//         Real sn = Math<Real>::Sin(angle);
//         if (Math<Real>::FAbs(sn) >= Math<Real>::ZERO_TOLERANCE)
//         {
//             Real coeff = angle/sn;
//             for (i = 1; i < 4; ++i)
//             {
//                 result.mTuple[i] = coeff*mTuple[i];
//             }
//             return result;
//         }
//     }
// 
//     for (i = 1; i < 4; ++i)
//     {
//         result.mTuple[i] = mTuple[i];
//     }
//     return result;
// }

Quaternion operator+(const Quaternion& lhs, const Quaternion& rhs)
{
    Quaternion ret(lhs);
    ret += rhs;
    return ret;
}

Quaternion operator-(const Quaternion& lhs, const Quaternion& rhs)
{
    Quaternion ret(lhs);
    ret -= rhs;
    return ret;
}

Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs)
{
    Quaternion ret(lhs);
    ret *= rhs;
    return ret;
}

Quaternion operator*(const Quaternion& lhs, const float t)
{
    Quaternion ret(lhs);
    ret *= t;
    return ret;
}

Quaternion operator*(const float t, const Quaternion& rhs)
{
    Quaternion ret(rhs);
    ret *= t;
    return ret;
}

bool operator==(const Quaternion& lhs, const Quaternion& rhs)
{
    return (lhs.w == rhs.w) && (lhs.y == rhs.y) && (lhs.z == lhs.z) && (lhs.x == rhs.x);
}

bool operator!=(const Quaternion& lhs, const Quaternion& rhs)
{
    return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& os, const Quaternion& q)
{
    os << "(" << q.w << ", " << q.x << ", " << q.y << ", " << q.z << ")";
    return os;
}

float DotProduct(const Quaternion& q1, const Quaternion& q2)
{
    return q1.w*q2.w + q1.x*q2.x + q1.y*q2.y + q1.z*q2.z;
}
