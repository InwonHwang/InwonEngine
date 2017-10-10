#pragma once
#include "..\Core\Core.h"

#define PLANE_EPSILON 10.0f

class Frustum
{
public:
	Frustum();
	~Frustum();

	bool Make(const D3DXMATRIX& matViewProj);
	bool IsIn(const Vector3& position) const;						// 정점 프러스텀 컬링
	bool IsInSphere(const Vector3& position, float radius) const;	// 경계구 프러스텀 컬링

	void Draw(IDirect3DDevice9* pDevice);							// debug용
	Vector3 GetCameraPos() const;
	
private:
	Vector3*	_pVertice;
	D3DXPLANE*	_pPlanes;
};

inline Vector3 Frustum::GetCameraPos() const
{
	return (_pVertice[0] + _pVertice[5] / 2.0f);
}

