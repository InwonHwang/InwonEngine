#pragma once

#include "..\Core\Core.h"
#include "..\Resource\Resource.h"

class UI abstract : public ComponentBase
{
public:
	UI(const shared_ptr<GameObject> gameObject) : ComponentBase(gameObject) {}
	virtual ~UI() {}

	virtual void Update(void* pData) abstract;
	virtual void Create(void* pData) abstract;
	virtual void Destroy() abstract;

	virtual void Draw(ID3DXSprite* pSprite) abstract;
};

