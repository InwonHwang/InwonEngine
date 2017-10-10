#pragma once
#include "PrimitiveCollider.h"

class CapsuleCollider final : public PrimitiveCollider//, public std::enable_shared_from_this<CapsuleCollider>
{
	enum Direction
	{
		eY,
		eX,
		eZ
	};
public:
	CapsuleCollider() : PrimitiveCollider() {}
	~CapsuleCollider() {}

	virtual void Update(void* pData) override;
	virtual void Create(void* pData, const shared_ptr<void> gameObject) override;
	virtual void Destroy() override;

	virtual void DebugDraw(IDirect3DDevice9* pd3dDevice) override;

private:
	void updateShape(float radius, float height);
};