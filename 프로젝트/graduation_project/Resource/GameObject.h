#pragma once

#include "..\Core\Core.h"
#include "..\Singleton\Singleton.h"

class GameObject : public ResourceBase, public enable_shared_from_this<GameObject>
{
	typedef vector<shared_ptr<ComponentBase>> ComponentVec;
private:
	shared_ptr<GameObject> root;

public:
	template<typename T>
	static shared_ptr<vector<shared_ptr<GameObject>>> FindObjectsOfType();
	static shared_ptr<GameObject> Instantiate(shared_ptr<GameObject> prefab);
	static shared_ptr<GameObject> Find(const string& name);
	static shared_ptr<GameObject> Create();
	static void Destroy(shared_ptr<GameObject>& gameObject);

private:
	unique_ptr<ComponentVec>	_components;

public:
	GameObject();
	~GameObject();

	virtual bool Create(void* pData, void* pResourceData, const string& name) override;
	virtual void Destroy() override;

	template <typename T>
	shared_ptr<T> AddComponent();

	
	shared_ptr<ComponentBase> AddComponent(const string& name);

	template <typename T>
	shared_ptr<T> GetComponent() const;

	template <typename T>
	shared_ptr<vector<shared_ptr<T>>> GetComponents() const;

	void SetName(const string& name) { _name = make_shared<string>(name); }
};

template<typename T>
inline shared_ptr<vector<shared_ptr<GameObject>>> GameObject::FindObjectsOfType()
{
	return GameObjectManager::Instance()->FindObjectsOfType<T>();
}

template<typename T>
inline shared_ptr<T> GameObject::AddComponent()
{
	static_assert(std::is_base_of<ComponentBase, T>::value, "");

	shared_ptr<T> component = make_shared<T>();
	
	component->Create(nullptr, shared_from_this());

	_components->push_back(component);

	return component;
}


inline shared_ptr<ComponentBase> GameObject::AddComponent(const string& name)
{
	auto pComponent = static_cast<ComponentBase*>(CREATE_TYPE_BY_NAME(name));

	shared_ptr<ComponentBase> component(pComponent, [&](void* p) {
		DESTROY_TYPE_BY_NAME(name, p);
	});

	component->Create(nullptr, shared_from_this());

	_components->push_back(component);

	return component;
}

template<typename T>
inline shared_ptr<T> GameObject::GetComponent() const
{
	static_assert(std::is_base_of<ComponentBase, T>::value, "");

	for (auto c : *_components)
	{
		shared_ptr<T> ret = std::dynamic_pointer_cast<T>(c);

		if (ret) return ret;
	}

	return nullptr;
}

template<typename T>
inline shared_ptr<vector<shared_ptr<T>>> GameObject::GetComponents() const
{
	shared_ptr<vector<shared_ptr<T>>> components(new vector<shared_ptr<T>>());

	for (auto c : *_components)
	{
		shared_ptr<T> ret = std::dynamic_pointer_cast<T>(c);
		
		if (ret) components->push_back(ret);
	}

	return components;
}