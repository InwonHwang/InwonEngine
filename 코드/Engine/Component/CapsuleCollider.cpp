#include "CapsuleCollider.h"
#include "RigidBody.h"
#include "Transform.h"

CapsuleCollider::CapsuleCollider(const shared_ptr<GameObject> gameObject) : Collider(gameObject)
{
}

CapsuleCollider::~CapsuleCollider()
{
}

void CapsuleCollider::Update(void* pData)
{
}

void CapsuleCollider::Create(void* pData)
{
}

void CapsuleCollider::Destroy()
{
}

void CapsuleCollider::DebugDraw(IDirect3DDevice9* pd3dDevice)
{
}