#pragma once

#include "..\Core\Core.h"

class GeometryResource;

class Mesh : public ResourceBase
{
	typedef vector<shared_ptr<GeometryResource>> MeshVec;

private:
	unique_ptr<MeshVec> _meshes;

public:
	Mesh();
	~Mesh();

	virtual bool Create(void* pData, void* pResourceData, const string& name) override;
	virtual void Destroy() override;

	const shared_ptr<GeometryResource> GetSubMesh(int index) const { return _meshes->data()[index]; }
	int GetSubMeshCount() const { return _meshes->size(); }
};