#include "Transform.h"

Transform::Transform(const shared_ptr<GameObject> gameObject)
	: ComponentBase(gameObject),
	_worldMatrix(nullptr),
	_scale(nullptr),
	_rotation(nullptr),
	_position(nullptr),
	_children(nullptr)
{
	_worldMatrix = make_shared<D3DXMATRIX>();
	_scale = make_shared<Vector3>(1.0f, 1.0f, 1.0f);
	_rotation = make_shared<Quaternion>(0.0f, 0.0f, 0.0f, 0.0f);
	_position = make_shared<Vector3>(0.0f, 0.0f, 0.0f);
	_children = make_unique<TransformList>();

	D3DXMatrixIdentity(_worldMatrix.get());
}

Transform::~Transform()
{
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

const shared_ptr<Vector3> Transform::getAxis(const Vector3& axis) const
{
	shared_ptr<Vector3> out = make_shared<Vector3>();

	D3DXMATRIX matRot;
	D3DXMatrixRotationQuaternion(&matRot, _rotation.get());

	D3DXVec3TransformCoord(out.get(), &axis, &matRot);
	D3DXVec3Normalize(out.get(), out.get());

	return out;
}

void Transform::Update(void* pData)
{
}

void Transform::Create(void* pData)
{
	if (!pData) return;

	Json::Value* pInfo = static_cast<Json::Value*>(pData);

	FromJson(*pInfo);
}

void Transform::Destroy()
{
}

shared_ptr<void> Transform::Clone()
{ 
	shared_ptr<Transform> transform = make_shared<Transform>(nullptr);

	transform->_worldMatrix = make_shared<D3DXMATRIX>(*_worldMatrix);
	transform->_scale = make_shared<Vector3>(*_scale);
	transform->_rotation = make_shared<Quaternion>(*_rotation);
	transform->_position = make_shared<Vector3>(*_position);

	return transform;
}

void Transform::FromJson(const Json::Value& json)
{
	SetLocalScale(Vector3(json["scale"]["x"].asFloat(),
		json["scale"]["y"].asFloat(),
		json["scale"]["z"].asFloat()));

	SetLocalRotation(Quaternion(json["rotation"]["x"].asFloat(),
		json["rotation"]["y"].asFloat(),
		json["rotation"]["z"].asFloat(),
		json["rotation"]["w"].asFloat()));

	SetLocalPosition(Vector3(json["translation"]["x"].asFloat(),
		json["translation"]["y"].asFloat(),
		json["translation"]["z"].asFloat()));
}

void Transform::ToJson(Json::Value& json)
{
	json["scale"]["x"] = _scale->x;
	json["scale"]["y"] = _scale->y;
	json["scale"]["z"] = _scale->z;

	json["rotation"]["x"] = _rotation->x;
	json["rotation"]["y"] = _rotation->y;
	json["rotation"]["z"] = _rotation->z;
	json["rotation"]["w"] = _rotation->w; 

	json["translation"]["x"] = _position->x;
	json["translation"]["y"] = _position->y;
	json["translation"]["z"] = _position->z;
}

const shared_ptr<Vector3> Transform::GetScale() const
{
	if (_parent.expired()) return GetLocalScale();

	auto parentScale = _parent.lock()->GetScale();

	return make_shared<Vector3>(_scale->x * parentScale->x, _scale->y * parentScale->y, _scale->z * parentScale->z);
}

const shared_ptr<Quaternion> Transform::GetRotation() const
{
	if (_parent.expired()) return GetLocalRotation();

	auto localEulerAngle = Quaternion::ToEulerAngle(*_rotation);
	auto parentEulerAngle = Quaternion::ToEulerAngle(*(_parent.lock()->_rotation));
	auto worldEulerAngle = localEulerAngle + parentEulerAngle;

	return make_shared<Quaternion>(Quaternion::Euler(worldEulerAngle.x, worldEulerAngle.y, worldEulerAngle.z));
}

const shared_ptr<Vector3> Transform::GetPosition() const
{
	if (_parent.expired())	return GetLocalPosition();

	return make_shared<Vector3>(*_position + *_parent.lock()->GetPosition());
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
	else
		_parent.reset();
}

shared_ptr<Transform> Transform::FindChild(const string& path)
{
	queue<weak_ptr<Transform>> q;

	q.push(shared_from_this());

	while (!q.empty())
	{
		auto cur = q.front().lock(); q.pop();		

		if (cur->_gameObject->GetName()->compare(path) == 0) return cur;

		for (int i = 0; i < cur->GetChildCount(); ++i)
			q.push(cur->GetChild(i));		
	}

	return nullptr;
}

