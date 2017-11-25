#pragma once

#include "Collider.h"

class BoxCollider final : public Collider
{
public:
	BoxCollider(const shared_ptr<GameObject> gameObject) : Collider(gameObject) {}
	~BoxCollider() {}

	virtual void Update(void* pData) override;
	virtual void Create(void* pData) override;
	virtual void Destroy() override;

	virtual shared_ptr<void> Clone() override { return nullptr; }
	virtual void FromJson(const Json::Value& json) override {}
	virtual void ToJson(Json::Value& json) override {}

	virtual void DebugDraw(IDirect3DDevice9* pd3dDevice) override;

	void SetExtents(const Vector3& extents);
	const shared_ptr<Vector3> GetExtents() const;
};

