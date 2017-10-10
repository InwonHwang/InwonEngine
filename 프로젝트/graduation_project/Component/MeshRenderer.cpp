#include "MeshRenderer.h"


void MeshRenderer::Update(void* pData)
{
}

void MeshRenderer::Create(void* pData, const shared_ptr<void> gameObject)
{
	_gameObject = static_pointer_cast<GameObject>(gameObject);
	_materials.reset(new MaterialVec);
}

void MeshRenderer::Destroy()
{
	_gameObject.reset();
	_materials.reset();
	_meshContainer.reset();
}

void MeshRenderer::SetMesh(const shared_ptr<Mesh> mesh)
{
	_meshContainer = mesh;
}