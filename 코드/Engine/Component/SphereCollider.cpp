#include "SphereCollider.h"
#include "BoxCollider.h"
#include "RigidBody.h"
#include "MeshRenderer.h"

SphereCollider::SphereCollider(const shared_ptr<GameObject> gameObject)
	: Collider(gameObject)
{
}

SphereCollider::~SphereCollider()
{
}

void SphereCollider::Update(void* pData)
{
	Collider::Update(pData);
}

void SphereCollider::Create(void* pData)
{
}

void SphereCollider::Destroy()
{
	_gameObject.reset();
}

void SphereCollider::DebugDraw(IDirect3DDevice9* pd3dDevice)
{
}