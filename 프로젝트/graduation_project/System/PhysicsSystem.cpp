#include "PhysicsSystem.h"
#include "..\Physics\Physics.h"
#include "..\Component\Component.h"
#include "..\Resource\Resource.h"


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
	//shared_ptr<ComponentPool<RigidBody>> pPool = ComponentMgr->GetComponentPool<RigidBody>();
	auto rigidbody_objects = GameObject::FindObjectsOfType<RigidBody>();
	//shared_ptr<ComponentPool<Collider>> pBPool = ComponentMgr->GetComponentPool<Collider>();

	int numManifolds = _physicsWorld->GetBulletDynamicWorld()->getDispatcher()->getNumManifolds();
	for (int i = 0; i<numManifolds; i++)
	{
		btPersistentManifold* contactManifold = _physicsWorld->GetBulletDynamicWorld()->getDispatcher()->getManifoldByIndexInternal(i);
		btCollisionObject* obA = const_cast<btCollisionObject*>(contactManifold->getBody0());
		btCollisionObject* obB = const_cast<btCollisionObject*>(contactManifold->getBody1());

		if (obA == obB) continue;

		int numContacts = contactManifold->getNumContacts();
		for (int j = 0; j<numContacts; j++)
		{
			btManifoldPoint& pt = contactManifold->getContactPoint(j);
			if (pt.getDistance()<0.f)
			{
				shared_ptr<RigidBody> rigidBodyA;
				shared_ptr<RigidBody> rigidBodyB;

				/*int rigidBodyCount = pPool->GetComponentCount();
				for (int k = 0; k < rigidBodyCount; ++k)
				{
					shared_ptr<RigidBody> rigidBody = pPool->GetComponent(k);

					if ((btRigidBody*)obA == rigidBody->GetRigidBody().get())
						rigidBodyA = rigidBody;

					if ((btRigidBody*)obB == rigidBody->GetRigidBody().get())
						rigidBodyB = rigidBody;
				}

				shared_ptr<Collider> boxColliderA;
				shared_ptr<Collider> boxColliderB;*/

				/*int colliderCount = pBPool->GetComponentCount();
				for (int k = 0; k < colliderCount; ++k)
				{
					shared_ptr<Collider> collider = pBPool->GetComponent(k);

					if ((btGhostObject*)obA == collider->GetGhostObject().get())
					{
						boxColliderA = collider;
					}

					if ((btGhostObject*)obB == collider->GetGhostObject().get())
					{
						boxColliderB = collider;
					}
				}*/

				/*shared_ptr<vector<shared_ptr<Behaviour>>> behaviorVec;
				shared_ptr<Collider> pCollider;
				if (boxColliderA && rigidBodyB && boxColliderA->GetGameObject().get() != rigidBodyB->GetGameObject().get())
				{
					behaviorVec = boxColliderA->GetGameObject()->GetComponents<Behaviour>();

					for (int k = 0; k < behaviorVec->size(); ++k)
					{
						pCollider = rigidBodyB->GetGameObject()->GetComponent<Collider>();
						if(pCollider)
							behaviorVec->data()[k]->OnTrigger(pCollider);
					}
						
					behaviorVec = rigidBodyB->GetGameObject()->GetComponents<Behaviour>();

					for (int k = 0; k < behaviorVec->size(); ++k)
					{
						pCollider = boxColliderA->GetGameObject()->GetComponent<Collider>();
						if (pCollider)
							behaviorVec->data()[k]->OnTrigger(pCollider);
					}
				}

				else if (boxColliderB && rigidBodyA && boxColliderB->GetGameObject().get() != rigidBodyA->GetGameObject().get())
				{
					behaviorVec = boxColliderB->GetGameObject()->GetComponents<Behaviour>();

					for (int k = 0; k < behaviorVec->size(); ++k)
					{
						pCollider = rigidBodyA->GetGameObject()->GetComponent<Collider>();
						if (pCollider)
							behaviorVec->data()[k]->OnTrigger(pCollider);
					}
					

					behaviorVec = rigidBodyA->GetGameObject()->GetComponents<Behaviour>();

					for (int k = 0; k < behaviorVec->size(); ++k)
					{
						pCollider = boxColliderB->GetGameObject()->GetComponent<Collider>();
						if (pCollider)
							behaviorVec->data()[k]->OnTrigger(pCollider);
					}
					
				}*/
			}
		}
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
