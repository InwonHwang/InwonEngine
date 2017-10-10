#pragma once
#include "Collider.h"

class TerrainCollider final : public Collider//, public std::enable_shared_from_this<TerrainCollider>
{
public:
	TerrainCollider() : Collider() {}
	~TerrainCollider() {}

	virtual void Update(void* pData) override;
	virtual void Create(void* pData, const shared_ptr<void> gameObject) override;
	virtual void Destroy() override;

	virtual void DebugDraw(IDirect3DDevice9* pd3dDevice) override;
};