#pragma once

#include "Collider.h"

class PrimitiveCollider abstract : public Collider
{
public:
	PrimitiveCollider() : Collider() {}
	virtual ~PrimitiveCollider() {}

	virtual void Update(void* pData);
	virtual void Create(void* pData, const shared_ptr<void> gameObject);
	virtual void Destroy() abstract;

	virtual void DebugDraw(IDirect3DDevice9* pd3dDevice) abstract;
};
