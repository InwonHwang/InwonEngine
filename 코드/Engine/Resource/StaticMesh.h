#pragma once

#include "GeometryResource.h"

class StaticMesh : public GeometryResource
{
public:
	StaticMesh();
	~StaticMesh();

	virtual bool Create(void* pData, void* pResourceData, const string& name) override;
	virtual void Destroy() override;
	virtual int GetStride() const override { return sizeof(STATICMESHVERTEX); }
	virtual unsigned long GetFVF() const override { return STATICMESHVERTEX::FVF; }
};