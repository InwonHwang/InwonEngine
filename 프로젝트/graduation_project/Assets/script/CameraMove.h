#pragma once

#include "..\..\Engine.h"

class CameraMove : public Behaviour
{
public:
	CameraMove() : Behaviour() {}
	~CameraMove() {}

	// protected:
	virtual void Update(void* pData) {}
	virtual void Create(void* pData, const shared_ptr<void> gameObject) { Awake(); _gameObject = static_pointer_cast<GameObject>(gameObject); }
	virtual void Create(void* pData, shared_ptr<void> pComponentInfo, const shared_ptr<void> gameObject) {}
	virtual void Destroy() {}

	// protected:
	virtual void Awake() override;	
	virtual void Update() override;
	virtual void OnTrigger(shared_ptr<Collider> pCollider) override;

	//virtual void OnCollision(shared_ptr<RigidBody> gameObject) override;
	//virtual void Start() = 0;
	//virtual void OnEnable() = 0;
	//virtual void OnCollision(shared_ptr<GameObject> gameObject) = 0;
	//virtual void OnTrigger(shared_ptr<GameObject> gameObject) = 0;
	//virtual void OnPostRender() = 0;
	//virtual void OnPreRender() = 0;
};