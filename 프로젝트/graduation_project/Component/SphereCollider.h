#pragma once
#include "PrimitiveCollider.h"

class SphereCollider final : public PrimitiveCollider
{
public:
	SphereCollider() : PrimitiveCollider() {}
	~SphereCollider() {}

	virtual void Update(void* pData) override;
	virtual void Create(void* pData, const shared_ptr<void> gameObject) override;
	virtual void Destroy() override;

	virtual void DebugDraw(IDirect3DDevice9* pd3dDevice) override;
};

