#include "Collider.h"
#include "Transform.h"


Collider::Collider()
	:
	ComponentBase(),
	btGhostObject()
{
}

Collider::~Collider()
{
}

void Collider::Create(void* pData, const shared_ptr<void> gameObject)
{
	_gameObject = static_pointer_cast<GameObject>(gameObject);

	Vector3 position;
	Quaternion rotation;
	_gameObject->GetComponent<Transform>()->GetPosition(position);
	_gameObject->GetComponent<Transform>()->GetRotation(rotation);

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(position.x, position.y, position.z));
	transform.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w));

	this->setWorldTransform(transform);
	this->setCollisionFlags(this->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);

	PhysicsWorld::Instance()->GetBulletDynamicWorld()->addCollisionObject(this);
}

void Collider::Update(void* pData)
{
	btTransform transform;

	Vector3 position;
	Quaternion rotation;
	_gameObject->GetComponent<Transform>()->GetPosition(position);
	_gameObject->GetComponent<Transform>()->GetRotation(rotation);

	transform.setOrigin(btVector3(position.x, position.y, position.z));
	transform.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w));

	this->setWorldTransform(transform);
}