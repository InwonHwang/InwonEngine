#pragma once

#include "..\Core\Core.h"
#include "..\Physics\Physics.h"
#include "..\Resource\Resource.h"

class RigidBody final : public ComponentBase, protected btRigidBody
{
	friend class BoxCollider;
	friend class SphereCollider;

private:
	btTransform transformToBulletTransform();

public:
	RigidBody(const shared_ptr<GameObject> gameObject);
	~RigidBody();

	virtual void Update(void* pData) override;
	virtual void Create(void* pData) override;
	virtual void Destroy() override;

	virtual shared_ptr<void> Clone() override { return nullptr; }
	virtual void FromJson(const Json::Value& json) override {}
	virtual void ToJson(Json::Value& json) override {}

	void SetMass(float mass);
	void SetKinematic(bool flag);
	void SetGravity(bool flag);

	bool IsKinematic() const;
	bool UseGravity() const { return getGravity() != btVector3(0, 0, 0); }

	float GetMass() const { return this->getInvMass(); }
};