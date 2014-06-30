#include "stdafx.h"

#include <cassert>
#include "Color.h"

Color& Color::operator+=(const Color& c)
{
	r += c.r;
	g += c.g;
	b += c.b;
	return *this;
}

Color& Color::operator-=(const Color& c)
{
	r -= c.r;
	g -= c.g;
	b -= c.b;
	return *this;
}

Color& Color::operator*=(const Color& c)
{
	r *= c.r;
	g *= c.g;
	b *= c.b;
	return *this;
}

Color& Color::operator*=(float f)
{
	r *= f;
	g *= f;
	b *= f;
	return *this;
}

Color& Color::operator/=(float f)
{
	assert(f != 0);
	f = 1/f;
	r *= f;
	g *= f;
	b *= f;
	return *this;
}

Color Color::operator+() const
{
	return *this;
}

Color Color::operator-() const
{
	return Color(r, g, b, a);
}

void Color::Clamp()
{
	r = Clampf(r, 0.0f, 1.0f);	
	g = Clampf(g, 0.0f, 1.0f);	
	b = Clampf(b, 0.0f, 1.0f);	
}

bool operator==(const Color& lhs, const Color& rhs)
{
	if ( FloatEqual(lhs.r, rhs.r) && 
		FloatEqual(lhs.g, rhs.g) && 
		FloatEqual(lhs.b, rhs.b) && 
		FloatEqual(lhs.a, rhs.a) )
	{
		return true;
	}
	return false;
}

bool operator!=(const Color& lhs, const Color& rhs)
{
	return !(lhs == rhs);
}

//-------------------------------------------------------------------
Color operator + (const Color& lhs, const Color& rhs)
{
	Color ret(lhs);
	ret += rhs;
	return ret;
}

Color operator-(const Color& lhs, const Color& rhs)
{
	Color ret(lhs);
	ret -= rhs;
	return ret;
}

Color operator * (float f, const Color& c)
{
	Color ret(c);
	ret *= f;
	return ret;
}

Color operator * (const Color& c, float f)
{
	Color ret(c);
	ret *= f;
	return ret;
}

Color operator * (const Color& lhs, const Color& rhs)
{
	Color ret(lhs);
	ret *= rhs;
	return ret;
}

Color operator/(const Color& c, float f)
{
	Color ret(c);
	ret /= f;
	return ret;
}

Color LERP(const Color& c1, const Color& c2, float t)
{
	return Color(LERPf(c1.r, c2.r, t),
		LERPf(c1.g, c2.g, t),
		LERPf(c1.b, c2.b, t),
		LERPf(c1.a, c2.a, t));
}
