#pragma once

#include "..\Core\Core.h"

class GameObject;

class PhysicsWorld : public SingletonBase<PhysicsWorld>
{
	typedef vector<shared_ptr<btRigidBody>>		RigidBodyVec;
	typedef vector<shared_ptr<btGhostObject>>	GhostObjectVec;

private:
	shared_ptr<btDefaultCollisionConfiguration>		_collisionConfiguration;
	shared_ptr<btCollisionDispatcher>				_dispatcher;
	shared_ptr<btBroadphaseInterface>				_overlappingPairCache;
	shared_ptr<btSequentialImpulseConstraintSolver> _solver;
	shared_ptr<btDbvtBroadphase>					_broadPhase;
	shared_ptr<btGhostPairCallback>					_ghostPairCallback;
	shared_ptr<btDiscreteDynamicsWorld>				_world;

public:
	PhysicsWorld();
	~PhysicsWorld();

	bool Init() override;
	void Release() override;

	shared_ptr<btDiscreteDynamicsWorld> GetBulletDynamicWorld() const { return _world; }
};
