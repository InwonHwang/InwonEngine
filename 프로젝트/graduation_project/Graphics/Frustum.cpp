#include "Frustum.h"

Frustum::Frustum()
{
	_pVertice = new Vector3[8];
	_pPlanes = new D3DXPLANE[6];
}


Frustum::~Frustum()
{
	SafeDeleteArray<Vector3>(_pVertice);
	SafeDeleteArray<D3DXPLANE>(_pPlanes);
}

bool Frustum::Make(const D3DXMATRIX& matViewProj)
{
	assert(_pVertice && "null reference: _pVertice");
	assert(_pPlanes && "null reference: _pPlanes");

	_pVertice[0].x = -1.0f;	_pVertice[0].y = -1.0f;	_pVertice[0].z = 0.0f;
	_pVertice[1].x = 1.0f;	_pVertice[1].y = -1.0f;	_pVertice[1].z = 0.0f;
	_pVertice[2].x = 1.0f;	_pVertice[2].y = -1.0f;	_pVertice[2].z = 1.0f;
	_pVertice[3].x = -1.0f;	_pVertice[3].y = -1.0f;	_pVertice[3].z = 1.0f;
	_pVertice[4].x = -1.0f;	_pVertice[4].y = 1.0f;	_pVertice[4].z = 0.0f;
	_pVertice[5].x = 1.0f;	_pVertice[5].y = 1.0f;	_pVertice[5].z = 0.0f;
	_pVertice[6].x = 1.0f;	_pVertice[6].y = 1.0f;	_pVertice[6].z = 1.0f;
	_pVertice[7].x = -1.0f;	_pVertice[7].y = 1.0f;	_pVertice[7].z = 1.0f;

	D3DXMATRIX matInv;
	D3DXMatrixInverse(&matInv, NULL, &matViewProj);

	for (int i = 0; i < 8; i++)
	{
		D3DXVec3TransformCoord(&_pVertice[i], &_pVertice[i], &matInv);
	}

	D3DXPlaneFromPoints(&_pPlanes[0], _pVertice+4, _pVertice+7, _pVertice+6);	// 상 평면(top)
	D3DXPlaneFromPoints(&_pPlanes[1], _pVertice  , _pVertice+1, _pVertice+2);	// 하 평면(bottom)
	D3DXPlaneFromPoints(&_pPlanes[2], _pVertice  , _pVertice+4, _pVertice+5);	// 근 평면(near)
	D3DXPlaneFromPoints(&_pPlanes[3], _pVertice + 2, _pVertice + 6, _pVertice + 7);	// 원 평면(far)
	D3DXPlaneFromPoints(&_pPlanes[4], _pVertice, _pVertice + 3, _pVertice + 7);	// 좌 평면(left)
	D3DXPlaneFromPoints(&_pPlanes[5], _pVertice + 1, _pVertice + 5, _pVertice + 6);	// 우 평면(right)

	return true;
}

bool Frustum::IsIn(const Vector3& position) const
{
	assert(_pPlanes && "plane is not created");

	for (int i = 0; i < 6; i++)
	{
		if (D3DXPlaneDotCoord(&_pPlanes[i], &position) > PLANE_EPSILON)
		{
			return false;
		}
	}
	return true;
}

bool Frustum::IsInSphere(const Vector3& position, float radius) const
{
	assert(_pPlanes && "plane is not created");

	for (int i = 0; i < 6; i++)
	{
		if (D3DXPlaneDotCoord(&_pPlanes[i], &position) > radius + PLANE_EPSILON)
		{
			return false;
		}
	}
	return true;
}

void Frustum::Draw(IDirect3DDevice9* pDevice)
{
	WORD index[] = { 0, 1, 2,
		0, 2, 3,
		4, 7, 6,
		4, 6, 5,
		1, 5, 6,
		1, 6, 2,
		0, 3, 7,
		0, 7, 4,
		0, 4, 5,
		0, 5, 1,
		3, 7, 6,
		3, 6, 2 };

	D3DMATERIAL9 mtrl;
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));

	typedef struct tagVTX
	{
		D3DXVECTOR3	p;
	} VTX;

	VTX		vtx[8];

	for (int i = 0; i < 8; i++)
		vtx[i].p = _pVertice[i];

	pDevice->SetFVF(D3DFVF_XYZ);
	pDevice->SetStreamSource(0, NULL, 0, sizeof(VTX));
	pDevice->SetTexture(0, NULL);
	pDevice->SetIndices(0);

	// 파란색으로 상,하 평면을 그린다.
	pDevice->SetRenderState(D3DRS_LIGHTING, true);
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
	pDevice->SetMaterial(&mtrl);
	pDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 8, 4, index, D3DFMT_INDEX16, vtx, sizeof(vtx[0]));

	// 녹색으로 좌,우 평면을 그린다.
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	pDevice->SetMaterial(&mtrl);
	pDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 8, 4, index + 4 * 3, D3DFMT_INDEX16, vtx, sizeof(vtx[0]));

	// 붉은색으로 원,근 평면을 그린다.
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	pDevice->SetMaterial(&mtrl);
	pDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 8, 4, index + 8 * 3, D3DFMT_INDEX16, vtx, sizeof(vtx[0]));
}
