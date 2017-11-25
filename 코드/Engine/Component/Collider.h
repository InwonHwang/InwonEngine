#pragma once

#include "..\Core\Core.h"

class btGhostObject;

class Collider abstract : public ComponentBase, public btGhostObject
{
public:
	Collider(const shared_ptr<GameObject> gameObject);
	virtual ~Collider();

	virtual void Update(void* pData) override;
	virtual void Create(void* pData) override;
	virtual void Destroy() override;

	virtual void DebugDraw(IDirect3DDevice9* pd3dDevice) abstract;
};