#include "BoxCollider.h"
#include "RigidBody.h"
#include "MeshRenderer.h"
#include "Transform.h"


void BoxCollider::Update(void* pData)
{
	Collider::Update(pData);
}

void BoxCollider::Create(void* pData)
{
	Collider::Create(pData);

	btVector3 extents(1.0f, 1.0f, 1.0f);

	auto renderer = GetComponent<MeshRenderer>();

	if (renderer) {
		auto bounds = renderer->GetBounds();
		auto scale = GetComponent<Transform>()->GetScale();
		btVector3 extents(bounds->extents->x * scale->x, bounds->extents->y * scale->y, bounds->extents->z * scale->z);
	}

	setCollisionShape(new btBoxShape(extents));

	auto rigidbody = GetComponent<RigidBody>();
	if (rigidbody) rigidbody->setCollisionShape(getCollisionShape());

	PhysicsWorld::Instance()->GetBulletDynamicWorld()->addCollisionObject(this);
}

void BoxCollider::Destroy()
{
	SafeDelete<btBoxShape>(getCollisionShape());

	PhysicsWorld::Instance()->GetBulletDynamicWorld()->removeCollisionObject(this);

	_gameObject.reset();
}

void BoxCollider::DebugDraw(IDirect3DDevice9* pd3dDevice)
{
	BOXVERTEX vtx[8];

	//auto shape = static_cast<btBoxShape*>(getCollisionShape());
	auto center = GetComponent<MeshRenderer>()->GetBounds()->center;
	auto extents = GetComponent<MeshRenderer>()->GetBounds()->extents;

	float right = center->x + extents->x;
	float left = center->x - extents->x;
	float top = center->y + extents->y;
	float bottom = center->y - extents->y;
	float front = center->z + extents->z;
	float back = center->z - extents->z;
	
	vtx[0] = BOXVERTEX(right, top, front, 0xffffffff);
	vtx[1] = BOXVERTEX(right, top, back, 0xffffffff);
	vtx[2] = BOXVERTEX(left, top, front, 0xffffffff);
	vtx[3] = BOXVERTEX(left, top, back, 0xffffffff);
	vtx[4] = BOXVERTEX(right, bottom, front, 0xffffffff);
	vtx[5] = BOXVERTEX(right, bottom, back, 0xffffffff);
	vtx[6] = BOXVERTEX(left, bottom, front, 0xffffffff);
	vtx[7] = BOXVERTEX(left, bottom, back, 0xffffffff);

	INDEX idx[12]
	{
		{0,1,2}, {2,1,3},
		{4,0,6}, {6,0,2},
		{7,5,6}, {6,5,4},
		{3,1,7}, {7,1,5},
		{4,5,0}, {0,5,1},
		{3,7,2}, {2,7,6}
	};

	pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	
	pd3dDevice->SetTransform(D3DTS_WORLD, GetComponent<Transform>()->GetWorldMatrix().get());
	pd3dDevice->SetFVF(BOXVERTEX::FVF);
	pd3dDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 8, 12, idx, D3DFMT_INDEX32, vtx, sizeof(BOXVERTEX));

	pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void BoxCollider::SetExtents(const Vector3& extents)
{
	SafeDelete<btBoxShape>(getCollisionShape());

	setCollisionShape(new btBoxShape(btVector3(extents.x, extents.y, extents.z)));
}

const shared_ptr<Vector3> BoxCollider::GetExtents() const
{
	auto extents = static_cast<const btBoxShape*>(getCollisionShape())->getHalfExtentsWithMargin();

	return make_shared<Vector3>(extents.getX(), extents.getY(), extents.getZ());
}