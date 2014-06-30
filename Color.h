#ifndef COLOR_H
#define COLOR_H

#include "Math/common.h"

typedef DWORD COLOR;
// maps unsigned 8 bits/channel to COLOR
#define COLOR_ARGB(a,r,g,b) \
	((COLOR)(((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff))))
#define COLOR_RGBA(r,g,b,a)	COLOR_ARGB(a,r,g,b)
#define COLOR_XRGB(r,g,b)		COLOR_ARGB(0xff,r,g,b)

// maps floating point channels (0.f to 1.f range) to COLOR
#define COLOR_COLORVALUE(r,g,b,a) \
	COLOR_RGBA((DWORD)((r)*255.f),(DWORD)((g)*255.f),(DWORD)((b)*255.f),(DWORD)((a)*255.f))

class Color
{
public:
	float r, g, b, a;

	Color() {}
	Color(float red, float green, float blue, float alpha = 1.0f)
		: r(red), g(green), b(blue), a(alpha)
	{
	}

	// assignment operators
	Color& operator += (const Color& c);
	Color& operator -= (const Color& c);
	Color& operator *= (const Color& c); // Hadamard product
	Color& operator *= (float f);
	Color& operator /= (float f);

	// unary operators
	Color operator + () const;
	Color operator - () const;

	void Clamp();
};

//--------------------------------------------------------
Color operator + (const Color& lhs, const Color& rhs);
Color operator - (const Color& lhs, const Color& rhs);
Color operator * (float f, const Color& c);
Color operator * (const Color& c, float f);
Color operator * (const Color& lhs, const Color& rhs);
Color operator / (const Color& c, float f);

// equality
bool operator == (const Color& lhs, const Color& rhs);
bool operator != (const Color& lhs, const Color& rhs);

Color LERP(const Color& c1, const Color& c2, float t);
#endif // COLOR_H