#pragma once

class GameObject;

class ComponentBase abstract// : public std::enable_shared_from_this<ComponentBase>
{
protected:
	shared_ptr<GameObject>	_gameObject;
	bool					_isEnabled;

public:
	virtual ~ComponentBase() {}

	virtual void Update(void* pData) abstract;
	virtual void Create(void* pData, const shared_ptr<void> gameObject) abstract;
	virtual void Destroy() abstract;

	template<typename T>
	shared_ptr<T> GetComponent() const;

	shared_ptr<GameObject> GetGameObject() const;

	void SetEnable(bool flag);
	bool GetEnabled() const;
};

template<typename T>
inline shared_ptr<T> ComponentBase::GetComponent() const
{
	return _gameObject->GetComponent<T>();
}

inline shared_ptr<GameObject> ComponentBase::GetGameObject() const
{
	return _gameObject; 
}

inline void ComponentBase::SetEnable(bool flag)
{
	_isEnabled = flag;
}

inline bool ComponentBase::GetEnabled() const
{
	return _isEnabled;
}