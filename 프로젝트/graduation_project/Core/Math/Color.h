#pragma once

#include "..\..\pch.h"

typedef struct Color : public D3DXCOLOR
{
public:
	Color() : D3DXCOLOR() {}
	Color(int argb) : D3DXCOLOR(argb) {}
	Color(float r, float g, float b, float a) : D3DXCOLOR(r, g, b, a) {}

	Color& operator += (const Color&);
	Color& operator -= (const Color&);
	Color& operator *= (float);
	Color& operator /= (float);

	Color operator + (const Color&) const;
	Color operator - (const Color&) const;
	Color operator * (float) const;
	Color operator / (float) const;

	friend Color operator * (float, const Color&);

	bool operator == (const Color&) const;
	bool operator != (const Color&) const;
}Color;
