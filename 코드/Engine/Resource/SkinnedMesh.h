#pragma once
#include "GeometryResource.h"

class SkinnedMesh : public GeometryResource
{
private:
	shared_ptr<vector<D3DXMATRIX>> _offsetMatrices;

public:
	SkinnedMesh();
	~SkinnedMesh();
	
	virtual bool Create(void* pData, void* pResourceData, const string& name) override;
	virtual void Destroy() override;
	virtual int GetStride() const override { return sizeof(SKINNEDMESHVERTEX); }
	virtual unsigned long GetFVF() const override { return SKINNEDMESHVERTEX::FVF; }

	const shared_ptr<vector<D3DXMATRIX>> GetOffsetMatrices() const { return _offsetMatrices; }
};

