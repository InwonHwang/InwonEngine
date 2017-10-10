#pragma once
#include "StaticMesh.h"

class SkinnedMesh : public StaticMesh
{
	typedef vector<int> BoneIndexVec;
private:
	shared_ptr<vector<D3DXMATRIX>> _offsetMatrices;

public:
	SkinnedMesh() : StaticMesh() {}
	~SkinnedMesh() {}
	
	virtual bool Create(void* pData, void* pResourceData, const string& name) override;
	virtual void Destroy() override;
	virtual int GetStride() const override { return sizeof(SKINNEDMESHVERTEX); }
	virtual unsigned long GetFVF() const override { return SKINNEDMESHVERTEX::FVF; }

	shared_ptr<vector<D3DXMATRIX>> GetOffsetMatrices() const { return _offsetMatrices; }
};

