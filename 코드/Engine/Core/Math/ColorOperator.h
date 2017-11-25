#pragma once

#include "Color.h"

inline Color operator * (float f, const Color& c)
{
	return Color(c.r * f, c.g * f, c.b * f, c.a * f);
}

inline Color& Color::operator += (const Color& c)
{
	r += c.r;
	g += c.g;
	b += c.b;
	a += c.a;
	return (*this);
}

inline Color& Color::operator -= (const Color& c)
{
	r -= c.r;
	g -= c.g;
	b -= c.b;
	a -= c.a;
	return (*this);
}

inline Color& Color::operator *= (float f)
{
	r *= f;
	g *= f;
	b *= f;
	a *= f;
	return (*this);
}
inline Color& Color::operator /= (float f)
{
	r /= f;
	g /= f;
	b /= f;
	a /= f;
	return (*this);
}

inline Color Color::operator + (const Color& c) const
{
	return Color(r + c.r, g + c.g, b + c.b, a + c.a);
}

inline Color Color::operator - (const Color& c) const
{
	return Color(r - c.r, g - c.g, b - c.b, a - c.a);
}

inline Color Color::operator * (float f) const
{
	return Color(r * f, g * f, b * f, a * f);
}

inline Color Color::operator / (float f) const
{
	return Color(r / f, g / f, b / f, a / f);
}

inline bool Color::operator == (const Color& c) const
{
	return r == c.r && g == c.g && b == c.b && a == c.a;
}

inline bool Color::operator != (const Color& c) const
{
	return r != c.r || g != c.g || b != c.b || a != c.a;
}
