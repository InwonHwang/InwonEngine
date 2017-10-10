#include "BoxCollider.h"
#include "RigidBody.h"
#include "MeshRenderer.h"
#include "Transform.h"


void BoxCollider::Update(void* pData)
{
	Collider::Update(pData);
}

void BoxCollider::Create(void* pData, const shared_ptr<void> gameObject)
{
	Collider::Create(pData, gameObject);
}

void BoxCollider::Destroy()
{
	_gameObject.reset();
}

void BoxCollider::DebugDraw(IDirect3DDevice9* pd3dDevice)
{
	/*shared_ptr<BOXCOLLIDERINFO> pTempInfo = std::static_pointer_cast<BOXCOLLIDERINFO>(_pColliderInfo);

	BOXVERTEX vtx[8];
	float x, y, z;
	x = pTempInfo->center.x + pTempInfo->size.x;
	y = pTempInfo->center.y + pTempInfo->size.y;
	z = pTempInfo->center.z + pTempInfo->size.z;
	vtx[0] = BOXVERTEX(pTempInfo->center.x + pTempInfo->size.x, pTempInfo->center.y + pTempInfo->size.y, pTempInfo->center.z + pTempInfo->size.z, 0xffffffff);
	vtx[1] = BOXVERTEX(pTempInfo->center.x + pTempInfo->size.x, pTempInfo->center.y + pTempInfo->size.y, pTempInfo->center.z - pTempInfo->size.z, 0xffffffff);
	vtx[2] = BOXVERTEX(pTempInfo->center.x - pTempInfo->size.x, pTempInfo->center.y + pTempInfo->size.y, pTempInfo->center.z + pTempInfo->size.z, 0xffffffff);
	vtx[3] = BOXVERTEX(pTempInfo->center.x - pTempInfo->size.x, pTempInfo->center.y + pTempInfo->size.y, pTempInfo->center.z - pTempInfo->size.z, 0xffffffff);
	vtx[4] = BOXVERTEX(pTempInfo->center.x + pTempInfo->size.x, pTempInfo->center.y - pTempInfo->size.y, pTempInfo->center.z + pTempInfo->size.z, 0xffffffff);
	vtx[5] = BOXVERTEX(pTempInfo->center.x + pTempInfo->size.x, pTempInfo->center.y - pTempInfo->size.y, pTempInfo->center.z - pTempInfo->size.z, 0xffffffff);
	vtx[6] = BOXVERTEX(pTempInfo->center.x - pTempInfo->size.x, pTempInfo->center.y - pTempInfo->size.y, pTempInfo->center.z + pTempInfo->size.z, 0xffffffff);
	vtx[7] = BOXVERTEX(pTempInfo->center.x - pTempInfo->size.x, pTempInfo->center.y - pTempInfo->size.y, pTempInfo->center.z - pTempInfo->size.z, 0xffffffff);

	INDEX idx[12]
	{
		{0,1,2}, {2,1,3},
		{4,0,6}, {6,0,2},
		{7,5,6}, {6,5,4},
		{3,1,7}, {7,1,5},
		{4,5,0}, {0,5,1},
		{3,7,2}, {2,7,6}
	};

	Vector3 t;
	Quaternion q;
	D3DXMATRIX r, tm, qm;
	_gameObject->GetComponent<Transform>()->GetPosition(t);
	_gameObject->GetComponent<Transform>()->GetRotation(q);
	D3DXMatrixTranslation(&tm, t.x, t.y, t.z);
	D3DXMatrixRotationQuaternion(&qm, &q);

	r = qm * tm;

	pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	
	pd3dDevice->SetTransform(D3DTS_WORLD, &r);
	pd3dDevice->SetFVF(BOXVERTEX::FVF);
	pd3dDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 8, 12, idx, D3DFMT_INDEX32, vtx, sizeof(BOXVERTEX));

	pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);*/
}