#include "MeshRenderer.h"
#include "Transform.h"

MeshRenderer::MeshRenderer(const shared_ptr<GameObject> gameObject)
	: Renderer(gameObject),
	_mesh(nullptr),
	_bounds(nullptr)
{
	_bounds = make_shared<Bounds>();
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::initBounds(const shared_ptr<Mesh> mesh)
{
	int subMeshCount = mesh->GetSubMeshCount();

	Vector3 maxPoint(0, 0, 0), minPoint(0, 0, 0);

	for (int i = 0; i < subMeshCount; ++i)
	{
		auto vertices = mesh->GetSubMesh(i)->GetVertices();

		for (auto vertx : *vertices)
		{
			if (vertx.x > maxPoint.x) maxPoint.x = vertx.x;
			if (vertx.y > maxPoint.y) maxPoint.y = vertx.y;
			if (vertx.z > maxPoint.z) maxPoint.z = vertx.z;
			if (vertx.x < minPoint.x) minPoint.x = vertx.x;
			if (vertx.y < minPoint.y) minPoint.y = vertx.y;
			if (vertx.z < minPoint.z) minPoint.z = vertx.z;
		}
	}

	*_bounds->center = (maxPoint + minPoint) / 2;
	*_bounds->extents = maxPoint - *_bounds->center;
}

void MeshRenderer::Update(void* pData)
{
}

void MeshRenderer::Create(void* pData)
{
	if (!pData) return;

	Json::Value* pInfo = static_cast<Json::Value*>(pData);

	FromJson(*pInfo);
}

void MeshRenderer::Destroy()
{
	_gameObject.reset();
}

shared_ptr<void> MeshRenderer::Clone()
{ 
	shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>(nullptr);
	meshRenderer->_mesh = _mesh;
	meshRenderer->_materials->assign(_materials->begin(), _materials->end());
	meshRenderer->_bounds = _bounds; // ¼öÁ¤

	return meshRenderer;
}

void MeshRenderer::FromJson(const Json::Value& json)
{
	auto mesh = Resources->Create<Mesh>(json["mesh"].asString());
	if (mesh) {
		SetMesh(mesh);
		initBounds(mesh);
	}

	SetMaterialCount(json["materials"].size());

	int i = 0;
	for (auto materialInfo : json["materials"]) {
		auto material = Resources->Create<Material>(materialInfo.asString());

		if (!material) continue;

		SetMaterial(i++, material);
	}
}

void MeshRenderer::ToJson(Json::Value& json)
{
	json["is_enabled"] = _isEnabled;

	json["mesh"] = *_mesh->GetName();

	for (auto material : *_materials)
		json["materials"].append(*material->GetName());
}