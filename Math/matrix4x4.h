#ifndef MATRI4X4_H
#define MATRI4X4_H
#include "common.h"
#include "vector4f.h"
// Adapted from Ogre Matrix4
// DirectX compatible matrix, row major
class Matrix4x4
{
public:
	union
	{
		struct  
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		float m[4][4];	// row-major
	};
	
	static const Matrix4x4 ZERO;
	static const Matrix4x4 IDENTITY;

public:
	Matrix4x4();

	Matrix4x4(float m11, float m12, float m13, float m14,
			  float m21, float m22, float m23, float m24,
			  float m31, float m32, float m33, float m34,
			  float m41, float m42, float m43, float m44);
	explicit Matrix4x4(float afMat[16]);
	Matrix4x4(const Matrix4x4& mat);

	// access
	inline float* operator [] ( size_t iRow )
	{
		assert( iRow < 4 );
		return m[iRow];
	}

	inline const float* operator [] ( size_t iRow ) const
	{
		assert( iRow < 4 );
		return m[iRow];
	}

	float& operator() (size_t row, size_t col);
	float  operator() (size_t row, size_t col) const;

	// assignment operators
	Matrix4x4& operator =  (const Matrix4x4& rhs);
	Matrix4x4& operator += (const Matrix4x4& rhs);
	Matrix4x4& operator -= (const Matrix4x4& rhs);
	Matrix4x4& operator *= (float fScalar);
	Matrix4x4& operator *= (const Matrix4x4& rhs);
	Matrix4x4& operator /= (float fScalar);

	// unary operators
	Matrix4x4 operator + () const;
	Matrix4x4 operator - () const;

	void SetToIdentity();
	// geometric methods
	Matrix4x4 Adjoint() const;
 	float Determinant() const;
 	Matrix4x4 Transpose();
 	Matrix4x4 Inverse();

	// TODO
	// to_quaternion
};

/************************************************************************/
/* Arithmetic operators                                                 */
/************************************************************************/
Matrix4x4 operator + (const Matrix4x4& lhs, const Matrix4x4& rhs);
Matrix4x4 operator - (const Matrix4x4& lhs, const Matrix4x4& rhs);
Matrix4x4 operator * (const Matrix4x4& lhs, const Matrix4x4& rhs);
Matrix4x4 operator * (const Matrix4x4& mat, float t);
Matrix4x4 operator * (float t, const Matrix4x4& mat);
Matrix4x4 operator / (const Matrix4x4& mat, float t);

Vector4f operator * (const Vector4f& vec, const Matrix4x4& mat);

/************************************************************************/
/* transformation methods                                                    */
/************************************************************************/
// translation
Matrix4x4 Translate(float tx, float ty, float tz);
Matrix4x4 Translate(const Vector3f& tv);

// rotation
Matrix4x4 RotateX(float radian);
Matrix4x4 RotateY(float radian);
Matrix4x4 RotateZ(float radian);
//void RotateAxis(const Vector3f& axis, float radian);

// scale
Matrix4x4 Scale(float sx, float sy, float sz);
Matrix4x4 Scale(float s);

Matrix4x4 Ortho(float left, float right, float bottom, float top, float nearPlane, float farPlane);
Matrix4x4 Frustum(float left, float right, float bottom, float top, float nearPlane, float farPlane);
Matrix4x4 Perspective(float fovy, float aspectRatio, float nearPlane, float farPlane);
Matrix4x4 LookAt(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz);
Matrix4x4 LookAt(const Vector3f& eye, const Vector3f& center, const Vector3f& up);
Matrix4x4 ViewPort(UINT32 x, UINT32 y, UINT32 width, UINT32 height, UINT16 minz = 0, UINT16 maxz = 1);

/************************************************************************/
/* Equality operators                                                   */
/************************************************************************/
bool operator == (const Matrix4x4& lhs, const Matrix4x4& rhs);
bool operator != (const Matrix4x4& lhs, const Matrix4x4& rhs);

/************************************************************************/
/* Stream inserter                                                      */
/************************************************************************/
std::ostream& operator << (std::ostream& os, const Matrix4x4& mat);


#endif // MATRI4X4_H