#pragma once
#include "..\stdafx.h"
#include "Memory.h"
#include "ResourceBase.h"

class Pool
{
public:
	virtual ~Pool() {}

	virtual void Clear() abstract;
};

class UniquableResourcePool : Pool
{
	typedef vector<shared_ptr<ResourceBase>> ResourceVec;

protected:
	unique_ptr<ResourceVec> _resources;
};


template<typename T>
class ResourcePool : public Pool
{
	typedef map<size_t, shared_ptr<T>> ResourceMap;
	
private:
	unique_ptr<ResourceMap>	_resources;

public:
	ResourcePool() : _resources(make_unique<ResourceMap>()) {}	
	~ResourcePool() {}	

	void Clear() override;

	shared_ptr<T> Create(void* pData, void* pResourceData, const string& name);
};

template <typename T>
shared_ptr<T> ResourcePool<T>::Create(void* pData, void* pResourceData, const string& name)
{	
	static_assert(is_base_of<ResourceBase, T>::value, "");

	if (is_same<GameObject, T>()) {
		auto resource = make_shared<T>();		

		resource->Create(pData, pResourceData, name);

		return resource;
	}

	size_t hashCode = hash<string>()(name);

	auto it = _resources->find(hashCode);
	if (it != _resources->end()) return it->second;

	auto resource = make_shared<T>();

	if (!resource->Create(pData, pResourceData, name)) return nullptr;

	_resources->insert(make_pair(hashCode, resource));

	return resource;
}

template<typename T>
void ResourcePool<T>::Clear()
{	
	for (auto resource : *_resources)	
		resource.second->Destroy();	

	_resources->clear();
}
