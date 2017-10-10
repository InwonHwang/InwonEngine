#pragma once

#include "..\..\pch.h"

class Mathf abstract
{
public:
	static float Infinity() { return INFINITY; }
	static float RadToDeg() { return (float)(180 / 3.14159265358979323846); }
	static float DegToRad() { return (float)(3.14159265358979323846 / 180); }

	static float Abs(float f) { return abs(f); }

	static float Sin(float f) { return sinf(f); }
	static float Cos(float f) { return cosf(f); }
	static float Tan(float f) { return tanf(f); }

	static float Acos(float f) { return acosf(f); }
	static float Asin(float f) { return asinf(f); }
	static float Atan(float f) { return atanf(f); }
	static float Atan2(float y, float x) { return atan2f(y, x); }

	static float Sqrt(float f) { return sqrtf(f); }	
	static float Round(float f) { return roundf(f); }
	static float Exp(float f) { return expf(f); }
	static float Floor(float f) { return floorf(f); }
	static float Ceil(float f) { return ceilf(f); }
	static float Clamp(float f, float min, float max) { return std::fmax(min, std::fmin(f, max)); }
	static float Lerp(float f1, float f2, float amount) { return  f1 + (f2 - f1) * amount; }

	static bool EqualWithEpsilon(float f1, float f2) { return fabsf(f1 - f2) <= FLT_EPSILON; }
};

