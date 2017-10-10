#pragma once

#include "..\Core\Core.h"
#include "..\Resource\Resource.h"

class GameObject;
class Collider;
class RigidBody;

class Behaviour abstract : public ComponentBase
{
public:
	virtual ~Behaviour() {}

	// protected:
	virtual void Update(void* pData) abstract;
	virtual void Create(void* pData, const shared_ptr<void> gameObject) abstract;
	virtual void Create(void* pData, shared_ptr<void> pComponentInfo, const shared_ptr<void> gameObject) abstract;
	virtual void Destroy() abstract;

	// protected:
	virtual void Awake() = 0;	
	virtual void Update() = 0;
	virtual void OnTrigger(shared_ptr<Collider> gameObject) = 0;

	//virtual void Start() = 0;
	//virtual void OnEnable() = 0;
	//virtual void OnCollision(shared_ptr<RigidBody> pCollider) = 0;
	//virtual void OnPostRender() = 0;
	//virtual void OnPreRender() = 0;
};