#pragma once

#include "..\..\stdafx.h"

struct Vector3;

typedef struct Quaternion : public D3DXQUATERNION
{
public:
	Quaternion() : D3DXQUATERNION() {}
	Quaternion(const Quaternion& q) : D3DXQUATERNION(q) {}
	Quaternion(float x, float y, float z, float w) : D3DXQUATERNION(x, y, z, w) {}
	~Quaternion() {}

	Quaternion& operator += (const Quaternion&);
	Quaternion& operator -= (const Quaternion&);
	Quaternion& operator *= (const Quaternion&);
	Quaternion& operator *= (float);
	Quaternion& operator /= (float);

	Quaternion operator + (const Quaternion&) const;
	Quaternion operator - (const Quaternion&) const;
	Quaternion operator * (const Quaternion&) const;
	Quaternion operator * (float) const;
	Quaternion operator / (float) const;

	friend Quaternion operator * (float, const Quaternion&);

	bool operator == (const Quaternion&) const;
	bool operator != (const Quaternion&) const;

public:
	static Quaternion Euler(float x, float y, float z);
	static Vector3 ToEulerAngle(const Quaternion& q);
	static Vector3 Rotate(const Vector3&, float x, float y, float z);
	static void SLerp(Quaternion& out, const Quaternion& q1, const Quaternion& q2, float t);
}Quaternion;
