#pragma once
#include "..\Core\Core.h"

#define PLANE_EPSILON 10.0f

class Frustum
{
public:
	Frustum();
	~Frustum();

	bool Make(const D3DXMATRIX& matViewProj);
	bool IsIn(const Vector3& position) const;						// ���� �������� �ø�
	bool IsInSphere(const Vector3& position, float radius) const;	// ��豸 �������� �ø�

	void Draw(IDirect3DDevice9* pDevice);							// debug��
	Vector3 GetCameraPos() const;
	
private:
	Vector3*	_pVertice;
	D3DXPLANE*	_pPlanes;
};

inline Vector3 Frustum::GetCameraPos() const
{
	return (_pVertice[0] + _pVertice[5] / 2.0f);
}

