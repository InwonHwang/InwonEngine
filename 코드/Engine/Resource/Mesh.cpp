#include "Mesh.h"
#include "StaticMesh.h"
#include "SkinnedMesh.h"
#include "..\Singleton\Singleton.h"

Mesh::Mesh()
	: ResourceBase()
{
	_meshes = make_unique<MeshVec>();
	_name = make_shared<string>();
}

Mesh::~Mesh()
{
}

bool Mesh::Create(void* pData, void* pResourceData, const string& name)
{
	string* pPath = static_cast<string*>(pResourceData);
	*_name = name;

	Json::Value json;
	JsonUtility::ReadFromFile(*pPath, json);
	
	_meshes->resize(json.size());

	int i = 0;
	shared_ptr<GeometryResource> newMesh;
	for (auto mesh : json)
	{
		string meshName = *pPath + "_" + to_string(i);
		mesh["name"] = meshName;

		if (mesh["is_skinned"].asBool()) newMesh = Resources->CreateFromData<SkinnedMesh>(&mesh, meshName);
		else newMesh = Resources->CreateFromData<StaticMesh>(&mesh, meshName);

		_meshes->data()[i++] = newMesh;
	}

	return true;
}

void Mesh::Destroy()
{
}
