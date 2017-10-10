#pragma once

#include "..\pch.h"
#include "BitFlag.h"
#include "Type.h"

class ResourceBase abstract
{
protected:
	shared_ptr<string>	_name;

public:
	virtual ~ResourceBase() {}

	virtual bool Create(void* pData, void* pResourceData, const string& name) abstract;
	virtual void Destroy() abstract;
	
	shared_ptr<string> GetName() const { return _name; }
};
