#include "Math/quaternion.h"

Quaternion::Quaternion()
{

}

Quaternion::Quaternion(const Quaternion& q)
{

}

Quaternion::Quaternion(const float fW, const float fX, const float fY, const float fZ)
{

}

Quaternion::Quaternion(const Vector4f& v)
{

}

Quaternion& Quaternion::operator=(const Quaternion& q)
{

}

Quaternion& Quaternion::operator+=(const Quaternion& q)
{

}

Quaternion& Quaternion::operator-=(const Quaternion& q)
{

}

Quaternion& Quaternion::operator*=(const Quaternion& q)
{

}

Quaternion& Quaternion::operator*=(const float t)
{

}

Quaternion& Quaternion::operator/=(const float t)
{

}

Quaternion Quaternion::Conjugate() const
{

}

float Quaternion::Length() const
{

}

float Quaternion::SquaredLength() const
{

}

void Quaternion::Normalize()
{

}

Quaternion Quaternion::Inverse() const
{

}

Quaternion operator+(const Quaternion& lhs, const Quaternion& rhs)
{

}

Quaternion operator-(const Quaternion& lhs, const Quaternion& rhs)
{

}

Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs)
{

}

Quaternion operator*(const Quaternion& lhs, const float t)
{

}

Quaternion operator*(const float t, const Quaternion& rhs)
{

}

bool operator==(const Quaternion& lhs, const Quaternion& rhs)
{

}

bool operator!=(const Quaternion& lhs, const Quaternion& rhs)
{

}

std::ostream& operator<<(std::ostream& os, const Quaternion& q)
{

}

float DotProduct(const Quaternion& q1, const Quaternion& q2)
{

}
