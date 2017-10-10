#pragma once

#include "..\Core\Core.h"

class StaticMesh;

class Mesh : public ResourceBase
{
	typedef vector<shared_ptr<StaticMesh>> MeshVec;
private:
	shared_ptr<MeshVec> _meshes;

public:
	Mesh() {}
	~Mesh() {}

	virtual bool Create(void* pData, void* pResourceData, const string& name) override;
	virtual void Destroy() override;

	shared_ptr<StaticMesh> GetSubMesh(size_t index) const { return _meshes->data()[index]; }
	int GetSubMeshCount() const { return _meshes->size(); }

};