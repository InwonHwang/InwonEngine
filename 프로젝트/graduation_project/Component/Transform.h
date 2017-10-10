#pragma once

#include "..\Core\Core.h"
#include "..\Resource\Resource.h"

class GameObject;

class Transform;

class Transform final : public ComponentBase, public std::enable_shared_from_this<Transform>
{
	bool weak_ptr_comparsion(const weak_ptr<Transform>& a, const weak_ptr<Transform>&  b) { return a.lock() == b.lock(); }

	typedef list<weak_ptr<Transform>> TransformList;
private:
	shared_ptr<D3DXMATRIX>		_localMatrix;
	shared_ptr<D3DXMATRIX>		_worldMatrix;

	shared_ptr<Vector3>			_scale;
	shared_ptr<Quaternion>		_rotation;
	shared_ptr<Vector3>			_position;

	weak_ptr<Transform>			_parent;
	shared_ptr<TransformList>	_children;

private:
	void setScale(const Vector3& s);
	void setRotation(const Quaternion& q);
	void setTranslation(const Vector3& t);

	void addChild(weak_ptr<Transform> child);
	void removeChild(weak_ptr<Transform> child);

	void getAxis(const Vector3& axis, Vector3& out) const;

public:
	Transform() {}
	~Transform() {}

	virtual void Update(void* pData) override;
	virtual void Create(void* pData, const shared_ptr<void> gameObject) override;
	virtual void Destroy() override;

	void SetLocalScale(const Vector3& inScale) { setScale(inScale); }
	void SetLocalRotation(const Quaternion& inRotation) { setRotation(inRotation); }
	void SetLocalPosition(const Vector3& inPosition) { setTranslation(inPosition); }

	void GetLocalScale(Vector3& outLocalScale) const { outLocalScale = *_scale;	}
	void GetLocalRotation(Quaternion& outLocalRotation) const {	outLocalRotation = *_rotation; }
	void GetLocalEulerAngle(Vector3& outLocalEulerAngle) const { outLocalEulerAngle = Quaternion::ToEulerAngle(*_rotation); }
	void GetLocalPosition(Vector3& outLocalPosition) const { outLocalPosition = *_position; }	

	void GetAxisX(Vector3& outAxisX) const { getAxis(Vector3(1.0f, 0.0f, 0.0f), outAxisX); }
	void GetAxisY(Vector3& outAxisY) const { getAxis(Vector3(0.0f, 1.0f, 0.0f), outAxisY); }
	void GetAxisZ(Vector3& outAxisZ) const { getAxis(Vector3(0.0f, 0.0f, 1.0f), outAxisZ); }

	void SetLocalMatrix(const D3DXMATRIX& m) { *_localMatrix = m; }
	void SetWorldMatrix(const D3DXMATRIX& m) { *_worldMatrix = m; }

	shared_ptr<D3DXMATRIX> GetWorldMatrix() { return _worldMatrix; }
	shared_ptr<D3DXMATRIX> GetLocalMatrix() const {	return _localMatrix; }

	void GetScale(Vector3& outScale) const;
	void GetRotation(Quaternion& outRotation) const;
	void GetEulerAngle(Vector3& outEulerAngle) const;
	void GetPosition(Vector3& outPosition) const;

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