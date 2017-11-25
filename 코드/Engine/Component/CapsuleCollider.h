#pragma once
#include "Collider.h"

class CapsuleCollider final : public Collider
{
	enum Direction
	{
		eY,
		eX,
		eZ
	};
public:
	CapsuleCollider(const shared_ptr<GameObject> gameObject);
	~CapsuleCollider();

	virtual void Update(void* pData) override;
	virtual void Create(void* pData) override;
	virtual void Destroy() override;

	virtual shared_ptr<void> Clone() override { return nullptr; }
	virtual void FromJson(const Json::Value& json) override {}
	virtual void ToJson(Json::Value& json) override {}

	virtual void DebugDraw(IDirect3DDevice9* pd3dDevice) override;
};