#pragma once

#include "..\..\pch.h"

typedef struct Vector2 : public D3DXVECTOR2
{
public:
	Vector2() : D3DXVECTOR2() {}
	Vector2(const Vector2& v) : D3DXVECTOR2(v) {}
	Vector2(const D3DXVECTOR2& v) : D3DXVECTOR2(v) {}
	Vector2(float x, float y) : D3DXVECTOR2(x, y) {}
	~Vector2() {}

	Vector2& operator += (const Vector2&);
	Vector2& operator -= (const Vector2&);
	Vector2& operator *= (float);
	Vector2& operator /= (float);

	Vector2 operator + (const Vector2&) const;
	Vector2 operator - (const Vector2&) const;
	Vector2 operator * (float) const;
	Vector2 operator / (float) const;

	friend Vector2 operator * (float, const Vector2&);

	bool operator == (const Vector2&) const;
	bool operator != (const Vector2&) const;
}Vector2;

typedef struct Vector3 : public D3DXVECTOR3
{
public:
	Vector3() : D3DXVECTOR3() {}
	Vector3(const Vector2& v) : D3DXVECTOR3(v) {}
	Vector3(const Vector3& v) : D3DXVECTOR3(v) {}
	Vector3(const D3DXVECTOR3& v) : D3DXVECTOR3(v) {}
	Vector3(float x, float y, float z) : D3DXVECTOR3(x, y, z) {}
	~Vector3() {}

	Vector3& operator += (const Vector3&);
	Vector3& operator -= (const Vector3&);
	Vector3& operator *= (float);
	Vector3& operator /= (float);

	Vector3 operator + (const Vector3&) const;
	Vector3 operator - (const Vector3&) const;
	Vector3 operator * (float) const;
	Vector3 operator / (float) const;

	friend Vector3 operator * (float, const Vector3&);

	bool operator == (const Vector3&) const;
	bool operator != (const Vector3&) const;

public:
	static void Lerp(Vector3& outVec, const Vector3& v1, const Vector3& v2, float frame);
	static float GetDistance(const Vector3& v1, const Vector3& v2);
	static float GetLength(const Vector3& v);
}Vector3;

typedef struct Vector4 : public D3DXVECTOR4
{
public:
	Vector4() : D3DXVECTOR4() {}
	Vector4(const Vector2& v) : D3DXVECTOR4(v) {}
	Vector4(const Vector3& v) : D3DXVECTOR4(v) {}
	Vector4(const Vector4& v) : D3DXVECTOR4(v) {}
	Vector4(const D3DXVECTOR4& v) : D3DXVECTOR4(v) {}
	Vector4(float x, float y, float z, float w) : D3DXVECTOR4(x, y, z, w) {}
	~Vector4() {}

	Vector4& operator += (const Vector4&);
	Vector4& operator -= (const Vector4&);
	Vector4& operator *= (float);
	Vector4& operator /= (float);

	Vector4 operator + (const Vector4&) const;
	Vector4 operator - (const Vector4&) const;
	Vector4 operator * (float) const;
	Vector4 operator / (float) const;

	friend Vector4 operator * (float, const Vector4&);

	bool operator == (const Vector4&) const;
	bool operator != (const Vector4&) const;
}Vector4;