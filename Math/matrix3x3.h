#ifndef MATRI3X3_H
#define MATRI3X3_H

#include "common.h"
#include <cstring>

// NB All code adapted from Wild Magic 0.2 Matrix math (free source code)
// http://www.geometrictools.com/

// NOTE.  The (x,y,z) coordinate system is assumed to be left-handed.
// Coordinate axis rotation matrices are of the form
//   RX =    1       0       0
//           0     cos(t)  sin(t)
//           0    -sin(t)  cos(t)
// where t > 0 indicates a counterclockwise rotation in the yz-plane
//   RY =  cos(t)    0    -sin(t)
//           0       1       0
//         sin(t)    0     cos(t)
// where t > 0 indicates a counterclockwise rotation in the zx-plane
//   RZ =  cos(t)  sin(t)    0
//        -sin(t)  cos(t)    0
//           0       0       1
// where t > 0 indicates a counterclockwise rotation in the xy-plane.
class Matrix3x3
{
public:
    union
    {
        struct  
        {
            float _11, _12, _13;
            float _21, _22, _23;
            float _31, _32, _33;
        };
        float m[3][3];	// row-major
    };

    static const Matrix3x3 ZERO;
    static const Matrix3x3 IDENTITY;

public:
    inline Matrix3x3() {}

    inline Matrix3x3(float m11, float m12, float m13,
        float m21, float m22, float m23,
        float m31, float m32, float m33)
    {
        m[0][0] = m11;	m[0][1] = m12;	m[0][2] = m13;
        m[1][0] = m21;	m[1][1] = m22;	m[1][2] = m23;
        m[2][0] = m31;	m[2][1] = m32;	m[2][2] = m33;
    }

    inline explicit Matrix3x3(float afMat[9])
    {
        memcpy(m, afMat, sizeof(float) * 9);
    }

    inline Matrix3x3(const Matrix3x3& mat)
    {
        memcpy(m, mat.m, sizeof(float) * 9);
    }

    // access
    inline float* operator [] ( size_t iRow )
    {
        assert( iRow < 3 );
        return m[iRow];
    }

    inline const float* operator [] ( size_t iRow ) const
    {
        assert( iRow < 3 );
        return m[iRow];
    }

    float& operator() (size_t row, size_t col);
    float  operator() (size_t row, size_t col) const;

    // assignment operators
    Matrix3x3& operator =  (const Matrix3x3& rhs);
    Matrix3x3& operator += (const Matrix3x3& rhs);
    Matrix3x3& operator -= (const Matrix3x3& rhs);
    Matrix3x3& operator *= (float fScalar);
    Matrix3x3& operator *= (const Matrix3x3& rhs);
    Matrix3x3& operator /= (float fScalar);

    // unary operators
    Matrix3x3 operator + () const;
    Matrix3x3 operator - () const;

    void SetToIdentity();
    float Determinant() const;
    Matrix3x3 Transpose();
    Matrix3x3 Inverse(float fTolerance);

private:
    bool Inverse (Matrix3x3& rkInverse, float fTolerance) const;
};

/************************************************************************/
/* Arithmetic operators                                                 */
/************************************************************************/
Matrix3x3 operator + (const Matrix3x3& lhs, const Matrix3x3& rhs);
Matrix3x3 operator - (const Matrix3x3& lhs, const Matrix3x3& rhs);
Matrix3x3 operator * (const Matrix3x3& lhs, const Matrix3x3& rhs);
Matrix3x3 operator * (const Matrix3x3& mat, float t);
Matrix3x3 operator * (float t, const Matrix3x3& mat);
Matrix3x3 operator / (const Matrix3x3& mat, float t);

#endif // MATRI3X3_H