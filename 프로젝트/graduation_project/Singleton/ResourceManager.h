#pragma once

#include "..\Core\Core.h"

class ResourceManager : public SingletonBase<ResourceManager>
{
	typedef vector<shared_ptr<Pool>> PoolVec;

private:
	IDirect3DDevice9*	_pd3dDevice;
	unique_ptr<PoolVec> _resourcePools;

private:
	template<typename T>
	shared_ptr<ResourcePool<T>> createResourcePool() const;

	template<typename T>
	shared_ptr<ResourcePool<T>> findResourcePool() const;

public:
	ResourceManager();
	~ResourceManager();

	bool Init() override;
	void Release() override;;

	template<typename T>
	shared_ptr<T> Create(const string& path);

	template<typename T>
	shared_ptr<T> CreateFromData(void* pResourceData, const string& name);
};

template<typename T>
shared_ptr<T> ResourceManager::Create(const string& path)
{
	shared_ptr<ResourcePool<T>> pResourcePool = findResourcePool<T>();

	if (!pResourcePool)
	{
		pResourcePool = createResourcePool<T>();
		_resourcePools->push_back(pResourcePool);
	}
	
	return pResourcePool->Create(_pd3dDevice, const_cast<string*>(&path), path);
}

template<typename T>
shared_ptr<T> ResourceManager::CreateFromData(void* pResourceData, const string& name)
{
	shared_ptr<ResourcePool<T>> pResourcePool = findResourcePool<T>();

	if (!pResourcePool)
	{
		pResourcePool = createResourcePool<T>();
		_resourcePools->push_back(pResourcePool);
	}

	return pResourcePool->Create(_pd3dDevice, pResourceData, name);
}

template<typename T>
shared_ptr<ResourcePool<T>> ResourceManager::createResourcePool() const
{
	shared_ptr<ResourcePool<T>> pResourcePool(new ResourcePool<T>());

	return pResourcePool;
}

template<typename T>
shared_ptr<ResourcePool<T>> ResourceManager::findResourcePool() const
{
	for (auto it = _resourcePools->begin(); it != _resourcePools->end(); ++it)
	{
		shared_ptr<ResourcePool<T>> pResourcePool = std::dynamic_pointer_cast<ResourcePool<T>>(*it);

		if (pResourcePool)
			return pResourcePool;
	}

	return nullptr;
}