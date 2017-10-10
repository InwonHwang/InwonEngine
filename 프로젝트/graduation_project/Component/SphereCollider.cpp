#include "SphereCollider.h"
#include "BoxCollider.h"
#include "RigidBody.h"
#include "MeshRenderer.h"


void SphereCollider::Update(void* pData)
{
	Collider::Update(pData);
}

void SphereCollider::Create(void* pData, const shared_ptr<void> gameObject)
{
	Collider::Create(pData, gameObject);
}

void SphereCollider::Destroy()
{
	_gameObject.reset();
}

void SphereCollider::DebugDraw(IDirect3DDevice9* pd3dDevice)
{
}