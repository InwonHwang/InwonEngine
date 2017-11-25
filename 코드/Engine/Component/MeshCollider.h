#pragma once
#include "Collider.h"

class Mesh;

class MeshCollider final : public Collider
{
private:
	shared_ptr<btTriangleMesh> _bulletMesh;

private:
	void createBulletMesh(const shared_ptr<Mesh> pMesh);

public:
	MeshCollider(const shared_ptr<GameObject> gameObject);
	~MeshCollider();

	virtual void Update(void* pData) override;
	virtual void Create(void* pData) override;
	virtual void Destroy() override;

	virtual shared_ptr<void> Clone() override { return nullptr; }
	virtual void FromJson(const Json::Value& json) override {}
	virtual void ToJson(Json::Value& json) override {}

	virtual void DebugDraw(IDirect3DDevice9* pd3dDevice) override;
};

