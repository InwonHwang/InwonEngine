#pragma once

#include "..\stdafx.h"
#include "BitFlag.h"
#include "Type.h"

class ResourceBase abstract
{
protected:
	shared_ptr<string>	_name;

public:
	ResourceBase() : _name(nullptr) { _name = make_shared<string>(); }
	virtual ~ResourceBase() {}

	virtual bool Create(void* pData, void* pResourceData, const string& name) abstract;
	virtual void Destroy() abstract;
	
	shared_ptr<string> GetName() const { return _name; }
};
