#pragma once

#include "..\Core\Core.h"
#include "..\Singleton\Singleton.h"

class GameObject : public ResourceBase, public ICloneableAndExportable, public enable_shared_from_this<GameObject>
{
	typedef vector<shared_ptr<ComponentBase>> ComponentVec;

public:
	template<typename T>
	static shared_ptr<list<shared_ptr<GameObject>>> FindObjectsOfType();
	static shared_ptr<GameObject> Instantiate(shared_ptr<GameObject> prefab);
	static shared_ptr<GameObject> Find(const string& name);
	static shared_ptr<GameObject> Create();
	static void Destroy(shared_ptr<GameObject>& gameObject);

private:
	unique_ptr<ComponentVec> _components;

private:
	void getJsonRecursively(const shared_ptr<GameObject> gameObject, Json::Value& json, int index, int parentIndex);
	void createCloneRecursively(const shared_ptr<GameObject> parent, const shared_ptr<GameObject> child);

public:
	GameObject();
	~GameObject();

	virtual bool Create(void* pData, void* pResourceData, const string& name) override;
	virtual void Destroy() override;

	virtual shared_ptr<void> Clone() override;
	virtual void Export(const string& path) override;
	virtual void FromJson(const Json::Value& json) override;
	virtual void ToJson(Json::Value& json) override;

	template <typename T>
	shared_ptr<T> AddComponent();

	shared_ptr<ComponentBase> AddComponent(const string& name);

	template <typename T>
	shared_ptr<T> GetComponent() const;

	template <typename T>
	shared_ptr<T> GetComponentInChildren() const;

	template <typename T>
	shared_ptr<vector<shared_ptr<T>>> GetComponents() const;

	void SetName(const string& name) { _name = make_shared<string>(name); }
};

template<typename T>
inline shared_ptr<list<shared_ptr<GameObject>>> GameObject::FindObjectsOfType()
{
	static_assert(std::is_base_of<ComponentBase, T>::value, "");

	auto rootGameObject = Scenes->GetCurrentScene()->GetRootGameObject();

	shared_ptr<list<shared_ptr<GameObject>>> ret = make_shared<list<shared_ptr<GameObject>>>();

	queue<shared_ptr<GameObject>> s({rootGameObject});

	while (!s.empty())
	{
		auto cur = s.front(); s.pop();
		auto transform = cur->GetComponent<Transform>();
		auto component = cur->GetComponent<T>();

		if (component) ret->push_back(cur);

		for (int i = 0; i < transform->GetChildCount(); ++i)
			s.push(transform->GetChild(i)->GetGameObject());
	}

	return ret;
}


inline shared_ptr<ComponentBase> GameObject::AddComponent(const string& name)
{
	auto pComponent = static_cast<ComponentBase*>(CREATE_TYPE_BY_NAME(name));

	shared_ptr<ComponentBase> component(pComponent, [&](void* p) {
		DESTROY_TYPE_BY_NAME(name, p);
	});

	_components->push_back(component);

	return component;
}

template<typename T>
inline shared_ptr<T> GameObject::AddComponent()
{
	static_assert(std::is_base_of<ComponentBase, T>::value, "");

	shared_ptr<T> component = make_shared<T>(shared_from_this());

	component->Create(nullptr);

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

template <typename T>
inline shared_ptr<T> GameObject::GetComponentInChildren() const
{
	static_assert(std::is_base_of<ComponentBase, T>::value, "");

	queue<shared_ptr<Transform>> q;

	auto transform = GetComponent<Transform>();

	for (int i = 0; i < transform->GetChildCount(); ++i)
		q.push(transform->GetChild(i));

	while (!q.empty())
	{
		auto cur = q.front(); q.pop();		
		auto component = cur->GetComponent<T>();

		if (component) return component;

		auto transform = cur->GetComponent<Transform>();

		for (int i = 0; i < transform->GetChildCount(); ++i)
			q.push(transform->GetChild(i));
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