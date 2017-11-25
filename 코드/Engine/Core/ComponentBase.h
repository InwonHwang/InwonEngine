#pragma once
#include "InterfaceBase.h"

class GameObject;

class ComponentBase abstract : public ICloneableAndJsonable
{
	friend class GameObject;
protected:
	shared_ptr<GameObject>	_gameObject;
	bool					_isEnabled;
	
public:
	ComponentBase(const shared_ptr<GameObject> gameObject) : _gameObject(gameObject), _isEnabled(true) {}
	virtual ~ComponentBase() { _gameObject.reset(); }

	virtual void Update(void* pData) abstract;
	virtual void Create(void* pData) abstract;
	virtual void Destroy() abstract;

	template<typename T>
	shared_ptr<T> GetComponent() const { return _gameObject->GetComponent<T>(); }

	void SetEnable(bool flag) { _isEnabled = flag; }
	bool GetEnabled() const { return _isEnabled; }

	shared_ptr<GameObject> GetGameObject() const { return _gameObject; }
};
