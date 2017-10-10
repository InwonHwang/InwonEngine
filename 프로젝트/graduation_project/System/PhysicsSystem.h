#pragma once

#include "..\Core\Core.h"

class PhysicsWorld;

class PhysicsSystem
{
public:
	PhysicsSystem(shared_ptr<PhysicsWorld>& physicsWorld);
	~PhysicsSystem();

public:
	void Update();
	

private:
	void checkCollision();
	void updateRigidBody();

	shared_ptr<PhysicsWorld> _physicsWorld;
};

