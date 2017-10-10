#include "CapsuleCollider.h"
#include "RigidBody.h"
#include "Transform.h"


void CapsuleCollider::Update(void* pData)
{
}

void CapsuleCollider::Create(void* pData, const shared_ptr<void> gameObject)
{
	Collider::Create(pData, gameObject);

	//_shape.reset(new btCapsuleShape(btScalar(pTempInfo->radius), btScalar(pTempInfo->height)));

}

void CapsuleCollider::Destroy()
{
	_gameObject.reset();
}

void CapsuleCollider::DebugDraw(IDirect3DDevice9* pd3dDevice)
{

}