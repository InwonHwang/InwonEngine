#include "RigidBody.h"
#include "PrimitiveCollider.h"
#include "Transform.h"

RigidBody::RigidBody()
	:
	ComponentBase(),
	btRigidBody(1.0f, new btDefaultMotionState(), new btBoxShape(btVector3(1.0f, 1.0f, 1.f)), btVector3(0,0,0))
{
}

RigidBody::~RigidBody()
{
}

void RigidBody::Update(void* pData)
{	
	auto pMotionState = this->getMotionState();
	btTransform transform;
	pMotionState->getWorldTransform(transform);
	btVector3 position = transform.getOrigin();
	btQuaternion quaternion = transform.getRotation();

	_gameObject->GetComponent<Transform>()->SetLocalPosition(Vector3(position.getX(), position.getY(), position.getZ()));
	_gameObject->GetComponent<Transform>()->SetLocalRotation(Quaternion(quaternion.getX(), quaternion.getY(), quaternion.getZ(), quaternion.getW()));
}

void RigidBody::Create(void* pData, const shared_ptr<void> gameObject)
{
	_gameObject = static_pointer_cast<GameObject>(gameObject);

	Vector3 t;
	Quaternion q;
	_gameObject->GetComponent<Transform>()->GetPosition(t);
	_gameObject->GetComponent<Transform>()->GetRotation(q);

	btTransform transform;
	transform.setOrigin(btVector3(t.x, t.y, t.z));
	transform.setRotation(btQuaternion(q.x, q.y, q.z, q.w));

	this->getMotionState()->setWorldTransform(transform);

	PhysicsWorld::Instance()->GetBulletDynamicWorld()->addRigidBody(this);
}

void RigidBody::Destroy()
{
	auto pMotionState = this->getMotionState();
	SafeDelete<btMotionState>(pMotionState);

	if (!_gameObject->GetComponent<Collider>())
	{
		auto pShape = this->getCollisionShape();
		SafeDelete<btCollisionShape>(pShape);
	}

	_gameObject.reset();

	PhysicsWorld::Instance()->GetBulletDynamicWorld()->removeRigidBody(this);	
}

void RigidBody::SetMass(float mass)
{
	this->setMassProps(mass, this->getLocalInertia());
}

void RigidBody::SetKinematic(bool flag)
{
	/*this->setCollisionFlags(this->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);

	int state = flag ? DISABLE_DEACTIVATION : ACTIVE_TAG;

	this->setActivationState(state);*/
}

void RigidBody::UseGravity(bool flag)
{
	if (!flag)	this->setGravity(btVector3(0, 0, 0));
	else		this->setGravity(btVector3(0, -9.81f, 0));
}

bool RigidBody::IsKinematic() const
{
	return true;
}
