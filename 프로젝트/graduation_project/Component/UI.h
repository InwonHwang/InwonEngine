#pragma once

#include "..\Core\Core.h"
#include "..\Resource\Resource.h"

class UI abstract : public ComponentBase
{
public:
	typedef vector<shared_ptr<Material>> MaterialVec;

public:
	UI() {}
	virtual ~UI() {}

	virtual void Update(void* pData) abstract;
	virtual void Create(void* pData, const shared_ptr<void> gameObject) abstract;
	virtual void Destroy() abstract;

	virtual void Draw(ID3DXSprite* pSprite) abstract;
};

