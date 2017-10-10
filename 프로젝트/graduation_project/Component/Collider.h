#pragma once

#include "..\Core\Core.h"
#include "..\Physics\Physics.h"
#include "..\Resource\Resource.h"

class Collider abstract : public ComponentBase, private btGhostObject
{
public:
	Collider();
	virtual ~Collider();

	virtual void Update(void* pData) override;
	virtual void Create(void* pData, const shared_ptr<void> gameObject) override;
	virtual void Destroy() abstract;

	virtual void DebugDraw(IDirect3DDevice9* pd3dDevice) abstract;
};