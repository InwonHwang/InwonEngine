#include "PhysicsWorld.h"

PhysicsWorld::PhysicsWorld()
{	
}

PhysicsWorld::~PhysicsWorld()
{	
}

bool PhysicsWorld::Init()
{
	_collisionConfiguration.reset(new btDefaultCollisionConfiguration());
	_dispatcher.reset(new btCollisionDispatcher(_collisionConfiguration.get()));
	_broadPhase.reset(new btDbvtBroadphase);
	_overlappingPairCache.reset(new btDbvtBroadphase());
	_solver.reset(new btSequentialImpulseConstraintSolver);
	_ghostPairCallback.reset(new btGhostPairCallback());
	_world.reset(new btDiscreteDynamicsWorld(_dispatcher.get(), _broadPhase.get(), _solver.get(), _collisionConfiguration.get()));

	_world->setGravity(btVector3(0, -9.81f, 0));
	_world->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(_ghostPairCallback.get());

	return true;
}

void PhysicsWorld::Release()
{
	_collisionConfiguration.reset();
	_dispatcher.reset();
	_broadPhase.reset();
	_overlappingPairCache.reset();
	_solver.reset();
	_ghostPairCallback.reset();
	_world.reset();
}