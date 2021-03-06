#pragma once
#include "Collider.h"

class SphereCollider final : public Collider
{
public:
	SphereCollider(const shared_ptr<GameObject> gameObject);
	~SphereCollider();

	virtual void Update(void* pData) override;
	virtual void Create(void* pData) override;
	virtual void Destroy() override;

	virtual shared_ptr<void> Clone() override { return nullptr; }
	virtual void FromJson(const Json::Value& json) override {}
	virtual void ToJson(Json::Value& json) override {}

	virtual void DebugDraw(IDirect3DDevice9* pd3dDevice) override;
};

