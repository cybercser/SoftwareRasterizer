#include "stdafx.h"

#include "matrix3x3.h"

const Matrix3x3 Matrix3x3::ZERO(0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 
    0.0f, 0.0f, 0.0f);

const Matrix3x3 Matrix3x3::IDENTITY(1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f);

float& Matrix3x3::operator()(size_t row, size_t col)
{
    assert(row < 3 && col < 3);
    return m[row][col];
}

float Matrix3x3::operator()(size_t row, size_t col) const
{
    assert(row < 3 && col < 3);
    return m[row][col];
}

Matrix3x3& Matrix3x3::operator=(const Matrix3x3& rhs)
{
    memcpy(m, rhs.m, sizeof(float) * 9);
    return *this;
}

Matrix3x3& Matrix3x3::operator+=(const Matrix3x3& rhs)
{
    for ( int i = 0; i < 3; ++i )
        for ( int j = 0; j < 3; ++j )
            m[i][j] += rhs.m[i][j];
    return *this;
}

Matrix3x3& Matrix3x3::operator-=(const Matrix3x3& rhs)
{
    for ( int i = 0; i < 3; ++i )
        for ( int j = 0; j < 3; ++j )
            m[i][j] -= rhs.m[i][j];
    return *this;
}

Matrix3x3& Matrix3x3::operator*=(float fScalar)
{
    for ( int i = 0; i < 3; ++i )
        for ( int j = 0; j < 3; ++j )
            m[i][j] *= fScalar;
    return *this;
}

Matrix3x3& Matrix3x3::operator*=(const Matrix3x3& rhs)
{
    Matrix3x3 temp;
    for ( int i = 0; i < 3; ++i )
    {
        for ( int j = 0; j < 3; ++j )
        {
            temp(i, j) = 0.0f;
            for ( int k = 0; k < 3; ++k )
                temp.m[i][j] += m[i][k] * rhs.m[k][j];
        }
    }
    memcpy(m, temp.m, sizeof(float) * 9);
    return *this;
}

Matrix3x3& Matrix3x3::operator/=(float fScalar)
{
    float recip = 1 / fScalar;
    for ( int i = 0; i < 3; ++i )
        for ( int j = 0; j < 3; ++j )
            m[i][j] *= recip;
    return *this;
}

Matrix3x3 Matrix3x3::operator+() const
{
    return *this;
}

Matrix3x3 Matrix3x3::operator-() const
{
    return *this * -1.0f;
}

void Matrix3x3::SetToIdentity()
{
    memset(m, 0, sizeof(float) * 9);
    m[0][0] = m[1][1] = m[2][2] = 1.0f;
}

float Matrix3x3::Determinant() const
{
    float fCofactor00 = m[1][1]*m[2][2] - m[1][2]*m[2][1];
    float fCofactor10 = m[1][2]*m[2][0] - m[1][0]*m[2][2];
    float fCofactor20 = m[1][0]*m[2][1] - m[1][1]*m[2][0];

    float fDet = m[0][0]*fCofactor00 + m[0][1]*fCofactor10 + m[0][2]*fCofactor20;

    return fDet;
}

Matrix3x3 Matrix3x3::Transpose()
{
    return Matrix3x3(m[0][0], m[1][0], m[2][0],
        m[0][1], m[1][1], m[2][1],
        m[0][2], m[1][2], m[2][2]);
}

Matrix3x3 Matrix3x3::Inverse(float fTolerance)
{
    Matrix3x3 kInverse = Matrix3x3::ZERO;
    Inverse(kInverse,fTolerance);
    return kInverse;
}

bool Matrix3x3::Inverse(Matrix3x3& rkInverse, float fTolerance) const
{
    // Invert a 3x3 using cofactors.  This is about 8 times faster than
    // the Numerical Recipes code which uses Gaussian elimination.

    rkInverse[0][0] = m[1][1]*m[2][2] - m[1][2]*m[2][1];
    rkInverse[0][1] = m[0][2]*m[2][1] - m[0][1]*m[2][2];
    rkInverse[0][2] = m[0][1]*m[1][2] - m[0][2]*m[1][1];
    rkInverse[1][0] = m[1][2]*m[2][0] - m[1][0]*m[2][2];
    rkInverse[1][1] = m[0][0]*m[2][2] - m[0][2]*m[2][0];
    rkInverse[1][2] = m[0][2]*m[1][0] - m[0][0]*m[1][2];
    rkInverse[2][0] = m[1][0]*m[2][1] - m[1][1]*m[2][0];
    rkInverse[2][1] = m[0][1]*m[2][0] - m[0][0]*m[2][1];
    rkInverse[2][2] = m[0][0]*m[1][1] - m[0][1]*m[1][0];

    float fDet = m[0][0]*rkInverse[0][0] + m[0][1]*rkInverse[1][0]+ m[0][2]*rkInverse[2][0];

    if ( fabs(fDet) <= fTolerance )
        return false;

    float fInvDet = 1.0f/fDet;
    for (size_t iRow = 0; iRow < 3; iRow++)
    {
        for (size_t iCol = 0; iCol < 3; iCol++)
            rkInverse[iRow][iCol] *= fInvDet;
    }

    return true;
}

/************************************************************************/
/* Arithmetic operators                                                 */
/************************************************************************/

Matrix3x3 operator+(const Matrix3x3& lhs, const Matrix3x3& rhs)
{
    Matrix3x3 ret(lhs);
    ret += rhs;
    return ret;
}

Matrix3x3 operator-(const Matrix3x3& lhs, const Matrix3x3& rhs)
{
    Matrix3x3 ret(lhs);
    ret -= rhs;
    return ret;
}

Matrix3x3 operator*(const Matrix3x3& lhs, const Matrix3x3& rhs)
{
    Matrix3x3 ret(lhs);
    ret *= rhs;
    return ret;
}

Matrix3x3 operator*(const Matrix3x3& mat, float t)
{
    Matrix3x3 ret(mat);
    ret *= t;
    return ret;
}

Matrix3x3 operator*(float t, const Matrix3x3& mat)
{
    Matrix3x3 ret(mat);
    ret *= t;
    return ret;
}

Matrix3x3 operator/(const Matrix3x3& mat, float t)
{
    float recip = 1 / t;
    Matrix3x3 ret(mat);
    ret *= recip;
    return ret;
}
