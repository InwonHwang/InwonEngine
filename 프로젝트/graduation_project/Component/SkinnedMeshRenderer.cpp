#include "SkinnedMeshRenderer.h"
#include "Transform.h"

void SkinnedMeshRenderer::Update(void* pData)
{
	IDirect3DDevice9* pd3dDevice = static_cast<IDirect3DDevice9*>(pData);
}

void SkinnedMeshRenderer::Create(void* pData, const shared_ptr<void> gameObject)
{
	_gameObject = static_pointer_cast<GameObject>(gameObject);
	_materials.reset(new MaterialVec);
	_bones.reset(new TransformList);
}

void SkinnedMeshRenderer::Destroy()
{
	_gameObject.reset();
	_materials.reset();
	_bones.reset();
	_meshContainer.reset();	
}


void SkinnedMeshRenderer::SetRootBone(const shared_ptr<Transform> pRoot)
{
	stack<shared_ptr<Transform>> s;

	s.push(pRoot);

	while (!s.empty())
	{
		auto temp = s.top();
		s.pop();

		_bones->push_back(temp);

		for (size_t i = 0; i < temp->GetChildCount(); ++i) {
			s.push(temp->GetChild(i));
		}
	}
}
