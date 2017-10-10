#include "Transform.h"

void Transform::setScale(const Vector3& s)
{ 
	*_scale = s;
}
void Transform::setRotation(const Quaternion& q)
{
	*_rotation = q; 
}

void Transform::setTranslation(const Vector3& t) 
{
	*_position = t; 
}

void Transform::addChild(weak_ptr<Transform> child) 
{
	_children->push_back(child);
}
void Transform::removeChild(weak_ptr<Transform> child) 
{ 
	_children->remove_if([child](const weak_ptr<Transform> p) { 
		return child.lock() == p.lock(); 
	}); 
}

void Transform::getAxis(const Vector3& axis, Vector3& out) const
{
	Quaternion rotation;
	GetRotation(rotation);

	D3DXMATRIX matRot;
	D3DXMatrixRotationQuaternion(&matRot, &rotation);

	D3DXVec3TransformCoord(&out, &axis, &matRot);
	D3DXVec3Normalize(&out, &out);
}

void Transform::Update(void* pData)
{
}

void Transform::Create(void* pData, const shared_ptr<void> gameObject)
{
	_gameObject = static_pointer_cast<GameObject>(gameObject);
	_localMatrix = make_shared<D3DXMATRIX>();
	_worldMatrix = make_shared<D3DXMATRIX>();
	_scale = make_shared<Vector3>(1.0f, 1.0f, 1.0f);
	_rotation = make_shared<Quaternion>(0, 0, 0, 0);
	_position = make_shared<Vector3>(0.0f, 0.0f, 0.0f);
	_children = make_shared<TransformList>();

	D3DXMatrixIdentity(_localMatrix.get());
	D3DXMatrixIdentity(_worldMatrix.get());
}

void Transform::Destroy()
{
	_localMatrix.reset();
	_worldMatrix.reset();
	_scale.reset();
	_rotation.reset();
	_position.reset();
	_children.reset();
}

void Transform::GetScale(Vector3& outScale) const
{
	if (_parent.expired()) {
		GetLocalScale(outScale);
		return;
	}

	outScale.x = _scale->x * _parent.lock()->_scale->x;
	outScale.y = _scale->y * _parent.lock()->_scale->y;
	outScale.z = _scale->z * _parent.lock()->_scale->z;
}

void Transform::GetRotation(Quaternion& outRotation) const
{
	if (_parent.expired()) {
		GetLocalRotation(outRotation);
		return;
	}

	auto localEulerAngle = Quaternion::ToEulerAngle(*_rotation);
	auto parentEulerAngle = Quaternion::ToEulerAngle(*(_parent.lock()->_rotation));
	auto worldEulerAngle = localEulerAngle + parentEulerAngle;
	outRotation = Quaternion::Euler(worldEulerAngle.x, worldEulerAngle.y, worldEulerAngle.z);
}

void Transform::GetEulerAngle(Vector3& outEulerAngle) const
{
	if (_parent.expired())
	{
		GetLocalEulerAngle(outEulerAngle);
		return;
	}

	auto localEulerAngle = Quaternion::ToEulerAngle(*_rotation);
	auto parentEulerAngle = Quaternion::ToEulerAngle(*_parent.lock()->_rotation);

	outEulerAngle.x = localEulerAngle.x + parentEulerAngle.x;
	outEulerAngle.y = localEulerAngle.y + parentEulerAngle.y;
	outEulerAngle.z = localEulerAngle.z + parentEulerAngle.z;
}

void Transform::GetPosition(Vector3& outPosition) const
{
	if (_parent.expired()) {
		GetLocalPosition(outPosition);
		return;
	}

	outPosition.x = _position->x + _parent.lock()->_position->x;
	outPosition.y = _position->y + _parent.lock()->_position->y;
	outPosition.z = _position->z + _parent.lock()->_position->z;
}

void Transform::SetParent(shared_ptr<Transform> newParant)
{
	if (!_parent.expired())
	{
		auto prevParent = _parent.lock();
		prevParent->removeChild(shared_from_this());
	}

	if (newParant)
	{
		newParant->addChild(shared_from_this());
		_parent = newParant;
	}
}

shared_ptr<Transform> Transform::FindChild(const string& path)
{
	stack<weak_ptr<Transform>> s;

	s.push(shared_from_this());

	while (!s.empty())
	{
		auto cur = s.top().lock();
		s.pop();

		if (cur->GetGameObject()->GetName()->compare(path) == 0) return cur;

		for (int i = 0; i < cur->GetChildCount(); ++i) {
			s.push(cur->GetChild(i));
		}
	}

	return nullptr;
}

