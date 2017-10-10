#pragma once

#include "..\Core\Core.h"
#include "..\Physics\Physics.h"
#include "..\Resource\Resource.h"

class RigidBody final : public ComponentBase, private btRigidBody
{
	friend class GameObject;
public:
	RigidBody();
	~RigidBody();

	virtual void Update(void* pData) override;
	virtual void Create(void* pData, const shared_ptr<void> gameObject) override;
	virtual void Destroy() override;

	void SetMass(float mass);
	void SetKinematic(bool flag);
	void UseGravity(bool flag);

	bool IsKinematic() const;
};