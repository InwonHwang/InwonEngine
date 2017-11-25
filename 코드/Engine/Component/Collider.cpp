#include "Collider.h"
#include "..\Physics\Physics.h"
#include "..\Resource\Resource.h"
#include "Transform.h"

Collider::Collider(const shared_ptr<GameObject> gameObject)
	: ComponentBase(gameObject),
	btGhostObject()
{
}

Collider::~Collider()
{
}

void Collider::Create(void* pData)
{
	auto p = GetComponent<Transform>()->GetPosition();
	auto q = GetComponent<Transform>()->GetRotation();

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(p->x, p->y, p->z));
	transform.setRotation(btQuaternion(q->x, q->y, q->z, q->w));

	setWorldTransform(transform);
	setCollisionFlags(this->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
}

void Collider::Destroy()
{
}

void Collider::Update(void* pData)
{
	btTransform transform;

	auto p = GetComponent<Transform>()->GetPosition();
	auto q = GetComponent<Transform>()->GetRotation();

	transform.setOrigin(btVector3(p->x, p->y, p->z));
	transform.setRotation(btQuaternion(q->x, q->y, q->z, q->w));

	setWorldTransform(transform);
}