#include "PhysicsSystem.h"
#include "..\Physics\Physics.h"
#include "..\Component\Component.h"
#include "..\Resource\Resource.h"
#include "..\Singleton\Singleton.h"


PhysicsSystem::PhysicsSystem(shared_ptr<PhysicsWorld>& physicsWorld)
{	
	_physicsWorld = physicsWorld;
}

PhysicsSystem::~PhysicsSystem()
{
	_physicsWorld.reset();
}

void PhysicsSystem::Update()
{
	_physicsWorld->GetBulletDynamicWorld()->stepSimulation(1.f / 60.f, 10);
	//checkCollision();
	updateRigidBody();
}

void PhysicsSystem::checkCollision()
{

	auto world = Physics->GetBulletDynamicWorld();
	auto colliderObjects = GameObject::FindObjectsOfType<Collider>();

	int numManifolds = world->getDispatcher()->getNumManifolds();

	for (int i = 0; i < numManifolds; i++)
	{
		btPersistentManifold* contactManifold = world->getDispatcher()->getManifoldByIndexInternal(i);
		const btCollisionObject* obA = contactManifold->getBody0();
		const btCollisionObject* obB = contactManifold->getBody1();

		int numContacts = contactManifold->getNumContacts();

		if (numContacts == 0) continue;

		//MessageBox(GetActiveWindow(), 0, 0, 0);

		/*for (int j = 0; j < numContacts; j++)
		{
			btManifoldPoint& pt = contactManifold->getContactPoint(j);
			if (pt.getDistance() < 0.f)
			{
				const btVector3& ptA = pt.getPositionWorldOnA();
				const btVector3& ptB = pt.getPositionWorldOnB();
				const btVector3& normalOnB = pt.m_normalWorldOnB;
			}
		}*/
	}	
}

void PhysicsSystem::updateRigidBody()
{
	auto rigidbodyObjects = GameObject::FindObjectsOfType<RigidBody>();

	for (auto rigidbodyObject : *rigidbodyObjects)
	{
		rigidbodyObject->GetComponent<RigidBody>()->Update(nullptr);
	}
}

void PhysicsSystem::updateCollider()
{
	auto colliderObjects = GameObject::FindObjectsOfType<Collider>();

	for (auto colliderObject : *colliderObjects)
	{
		colliderObject->GetComponent<Collider>()->Update(nullptr);
	}
}
