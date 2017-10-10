﻿#pragma once

#include "Quaternion.h"
#include "Mathf.h"
#include "Vector.h"

//http://stackoverflow.com/questions/11492299/quaternion-to-euler-angles-algorithm-how-to-convert-to-y-up-and-between-ha/11505219 ����

inline Quaternion Quaternion::Euler(float x, float y, float z)
{
	float x0 = x * Mathf::DegToRad();
	float y0 = y * Mathf::DegToRad();
	float z0 = z * Mathf::DegToRad();
	float t0 = Mathf::Sin(z0 * 0.5f);
	float t1 = Mathf::Cos(z0 * 0.5f);
	float t2 = Mathf::Sin(x0 * 0.5f);
	float t3 = Mathf::Cos(x0 * 0.5f);
	float t4 = Mathf::Sin(y0 * 0.5f);
	float t5 = Mathf::Cos(y0 * 0.5f);
	Quaternion q;
	q.x = t5 * t2 * t1 + t4 * t3 * t0;
	q.y = t4 * t3 * t1 - t5 * t2 * t0;
	q.z = t5 * t3 * t0 - t4 * t2 * t1;
	q.w = t5 * t3 * t1 + t4 * t2 * t0;
	return q;
}

inline Vector3 Quaternion::ToEulerAngle(const Quaternion& q)
{
	Vector3 v;

	v.x = Mathf::Atan2(2 * q.x*q.w - 2 * q.y*q.z, 1 - 2 * q.x*q.x - 2 * q.z*q.z) * Mathf::RadToDeg();
	v.y = Mathf::Atan2(2 * q.y*q.w - 2 * q.x*q.z, 1 - 2 * q.y*q.y - 2 * q.z*q.z) * Mathf::RadToDeg();
	v.z = Mathf::Asin(2 * q.x*q.y + 2 * q.z*q.w) * Mathf::RadToDeg();

	return v;
}

// http://stackoverflow.com/questions/14607640/rotating-a-vector-in-3d-space

inline Vector3 Quaternion::Rotate(const Vector3& v, float x, float y, float z)
{
	Vector3 ret = v;

	ret.x = ret.x * Mathf::Cos(z) - ret.y * Mathf::Sin(z);
	ret.y = ret.x * Mathf::Sin(z) + ret.y * Mathf::Cos(z);
	ret.z = ret.z;

	ret.x = ret.x * Mathf::Cos(y) + ret.z * Mathf::Sin(y);
	ret.z = ret.x * -Mathf::Sin(y) + ret.z * Mathf::Cos(y);

	ret.y = ret.y * Mathf::Cos(x) - ret.z * Mathf::Sin(x);
	ret.z = ret.y * Mathf::Sin(x) + ret.z * Mathf::Cos(x);

	return ret;
}

inline void Quaternion::SLerp(Quaternion& out, const Quaternion& q1, const Quaternion& q2, float t)
{
	D3DXQuaternionSlerp(&out, &q1, &q2, t);
}
