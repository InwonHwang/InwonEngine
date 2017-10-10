#include "ResourceManager.h"
#include "..\Graphics\Graphics.h"

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{	
}

bool ResourceManager::Init()
{
	_resourcePools = make_unique<PoolVec>();
	_pd3dDevice = D3D9Core::Instance()->GetD3DDevice();
	return true;
}

void ResourceManager::Release()
{
	for (auto it = _resourcePools->begin(); it != _resourcePools->end(); ++it)	
		(*it)->Clear();
}