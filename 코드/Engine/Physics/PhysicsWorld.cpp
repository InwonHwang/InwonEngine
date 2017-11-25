#include "PhysicsWorld.h"

PhysicsWorld::PhysicsWorld()
{	
}

PhysicsWorld::~PhysicsWorld()
{	
}

bool PhysicsWorld::Init()
{
	_collisionConfiguration = make_shared<btDefaultCollisionConfiguration>();
	_dispatcher = make_shared<btCollisionDispatcher>(_collisionConfiguration.get());
	_broadPhase = make_shared<btDbvtBroadphase>();
	_overlappingPairCache = make_shared<btDbvtBroadphase>();
	_solver = make_shared<btSequentialImpulseConstraintSolver>();
	_ghostPairCallback = make_shared<btGhostPairCallback>();
	_world = make_shared<btDiscreteDynamicsWorld>(_dispatcher.get(), _broadPhase.get(), _solver.get(), _collisionConfiguration.get());

	_world->setGravity(btVector3(0, -9.81f, 0));
	_world->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(_ghostPairCallback.get());

	return true;
}

void PhysicsWorld::Release()
{
}