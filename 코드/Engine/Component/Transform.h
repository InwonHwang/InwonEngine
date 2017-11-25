#pragma once

#include "..\Core\Core.h"
#include "..\Resource\Resource.h"

class GameObject;

class Transform final : public ComponentBase, public std::enable_shared_from_this<Transform>
{
	bool weak_ptr_comparsion(const weak_ptr<Transform>& a, const weak_ptr<Transform>&  b) { return a.lock() == b.lock(); }

	typedef list<weak_ptr<Transform>> TransformList;
private:
	shared_ptr<D3DXMATRIX>		_worldMatrix;

	shared_ptr<Vector3>			_scale;
	shared_ptr<Quaternion>		_rotation;
	shared_ptr<Vector3>			_position;

	weak_ptr<Transform>			_parent;
	unique_ptr<TransformList>	_children;

private:
	void addChild(weak_ptr<Transform> child);
	void removeChild(weak_ptr<Transform> child);

	const shared_ptr<Vector3> getAxis(const Vector3& axis) const;

public:
	Transform(const shared_ptr<GameObject> gameObject);
	~Transform();

	virtual void Update(void* pData) override;
	virtual void Create(void* pData) override;
	virtual void Destroy() override;

	virtual shared_ptr<void> Clone() override;
	virtual void FromJson(const Json::Value& json) override;
	virtual void ToJson(Json::Value& json) override;

	void SetLocalScale(const Vector3& inScale) { *_scale = inScale; }
	void SetLocalRotation(const Quaternion& inRotation) { *_rotation = inRotation; }
	void SetLocalPosition(const Vector3& inPosition) { *_position = inPosition; }

	const shared_ptr<Vector3> GetLocalScale() const { return _scale; }
	const shared_ptr<Quaternion> GetLocalRotation() const {	return _rotation; }
	const shared_ptr<Vector3> GetLocalPosition() const { return _position; }

	shared_ptr<Vector3> GetLocalEularAngle() const { return make_shared<Vector3>(Quaternion::ToEulerAngle(*GetLocalRotation())); }
	shared_ptr<Vector3> GetEularAngle() const { return make_shared<Vector3>(Quaternion::ToEulerAngle(*GetRotation())); }

	const shared_ptr<Vector3> GetAxisX() const { return getAxis(Vector3(1.0f, 0.0f, 0.0f)); }
	const shared_ptr<Vector3> GetAxisY() const { return getAxis(Vector3(0.0f, 1.0f, 0.0f)); }
	const shared_ptr<Vector3> GetAxisZ() const { return getAxis(Vector3(0.0f, 0.0f, 1.0f)); }

	void SetWorldMatrix(const D3DXMATRIX& m) { *_worldMatrix = m; }

	const shared_ptr<D3DXMATRIX> GetWorldMatrix() const { return _worldMatrix; }

	const shared_ptr<Vector3> GetScale() const;
	const shared_ptr<Quaternion> GetRotation() const;
	const shared_ptr<Vector3> GetPosition() const;

	void SetParent(shared_ptr<Transform> pParent);
	shared_ptr<Transform> FindChild(const string& path);

	int GetChildCount() const { return _children->size(); }

	shared_ptr<Transform> GetParent() const { return _parent.expired() ? nullptr : _parent.lock(); }
	shared_ptr<Transform> GetChild(int index) const
	{ 
		auto it = _children->begin();
		advance(it, index);

		return it == _children->end() ? nullptr : (*it).lock();
	}
};