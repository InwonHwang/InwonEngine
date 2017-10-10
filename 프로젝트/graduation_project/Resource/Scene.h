#pragma once

#include "..\Core\Core.h"

class Scene : public ResourceBase
{
private :
	shared_ptr<GameObject> _root;

public:
	Scene();
	virtual ~Scene();

	virtual bool Create(void* pData, void* pResourceData, const string& name) override;
	virtual void Destroy() override;
};

