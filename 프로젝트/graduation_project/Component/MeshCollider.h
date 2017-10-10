#pragma once
#include "Collider.h"

class Mesh;

class MeshCollider final : public Collider//, public std::enable_shared_from_this<MeshCollider>
{
public:
	MeshCollider() : Collider() {}
	~MeshCollider() {}

	virtual void Update(void* pData) override;
	virtual void Create(void* pData, const shared_ptr<void> gameObject) override;
	virtual void Destroy() override;

	virtual void DebugDraw(IDirect3DDevice9* pd3dDevice) override;

private:
	void createBulletMesh(const shared_ptr<Mesh> pMeshContainer);

private:
	shared_ptr<btTriangleMesh> _bulletMesh;
};

