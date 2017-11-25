#include "SkinnedMeshRenderer.h"
#include "Transform.h"

SkinnedMeshRenderer::SkinnedMeshRenderer(const shared_ptr<GameObject> gameObject)
	:
	MeshRenderer(gameObject),
	_bones(nullptr)
{
	_bones = make_shared<TransformVec>();
}

SkinnedMeshRenderer::~SkinnedMeshRenderer()
{
}

void SkinnedMeshRenderer::setBoneRecursively(shared_ptr<Transform> rootBone)
{
	queue<shared_ptr<Transform>> q({ rootBone });

	while (!q.empty())
	{
		auto bone = q.front(); q.pop();

		for (int i = 0; i < bone->GetChildCount(); ++i)
			q.push(bone->GetChild(i));
	}
}

void SkinnedMeshRenderer::Update(void* pData)
{
}

void SkinnedMeshRenderer::Create(void* pData)
{
	MeshRenderer::Create(pData);	
}

void SkinnedMeshRenderer::Destroy()
{
	MeshRenderer::Destroy();
}

shared_ptr<void> SkinnedMeshRenderer::Clone()
{
	shared_ptr<SkinnedMeshRenderer> skinnedMeshRenderer = make_shared<SkinnedMeshRenderer>(nullptr);
	skinnedMeshRenderer->_mesh = _mesh;
	skinnedMeshRenderer->_materials->assign(_materials->begin(), _materials->end());
	skinnedMeshRenderer->_bounds = _bounds; // ¼öÁ¤

	return skinnedMeshRenderer;
}

void SkinnedMeshRenderer::FromJson(const Json::Value& json)
{
	MeshRenderer::FromJson(json);
}

void SkinnedMeshRenderer::ToJson(Json::Value& json)
{
	MeshRenderer::ToJson(json);
	
	if(_rootBone)
		json["root_bone"] = *_rootBone->GetGameObject()->GetName();

	for (auto bone : *_bones)
		json["bones"].append(*bone->GetGameObject()->GetName());
}
