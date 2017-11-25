#include "RigidBody.h"
#include "Collider.h"
#include "Transform.h"

RigidBody::RigidBody(const shared_ptr<GameObject> gameObject)
	:
	ComponentBase(gameObject),
	btRigidBody(1.0f, new btDefaultMotionState(), new btBoxShape(btVector3(1.0f, 1.0f, 1.f)), btVector3(0,0,0))
{
}

RigidBody::~RigidBody()
{
}

btTransform RigidBody::transformToBulletTransform()
{
	btTransform transform;
	auto t = GetComponent<Transform>()->GetPosition();
	auto q = GetComponent<Transform>()->GetRotation();

	transform.setOrigin(btVector3(t->x, t->y, t->z));
	transform.setRotation(btQuaternion(q->x, q->y, q->z, q->w));

	return transform;
}

void RigidBody::Update(void* pData)
{	
	btTransform transform;
	getMotionState()->getWorldTransform(transform);

	auto p = transform.getOrigin();
	auto q = transform.getRotation();

	GetComponent<Transform>()->SetLocalPosition(Vector3(p.getX(), p.getY(), p.getZ()));
	GetComponent<Transform>()->SetLocalRotation(Quaternion(q.getX(), q.getY(), q.getZ(), q.getW()));
}

void RigidBody::Create(void* pData)
{
	btTransform transform = transformToBulletTransform();

	getMotionState()->setWorldTransform(transform);

	PhysicsWorld::Instance()->GetBulletDynamicWorld()->addRigidBody(this);
}

void RigidBody::Destroy()
{
	SafeDelete<btMotionState>(getMotionState());
	SafeDelete<btCollisionShape>(getCollisionShape());

	PhysicsWorld::Instance()->GetBulletDynamicWorld()->removeRigidBody(this);

	_gameObject.reset();
}

void RigidBody::SetMass(float mass)
{
	setMassProps(mass, getLocalInertia());
}

void RigidBody::SetKinematic(bool flag)
{
	setCollisionFlags(getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);

	setActivationState(flag ? DISABLE_DEACTIVATION : ACTIVE_TAG);
}

void RigidBody::SetGravity(bool flag)
{
	setGravity(flag ? btVector3(0, -9.81f, 0) : btVector3(0, 0, 0));
}

bool RigidBody::IsKinematic() const
{
	return isKinematicObject();
}
