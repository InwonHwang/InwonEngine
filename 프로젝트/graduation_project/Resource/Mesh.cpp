#include "Mesh.h"
#include "SkinnedMesh.h"
#include "..\Singleton\Singleton.h"

bool Mesh::Create(void* pData, void* pResourceData, const string& name)
{
	string* pPath = static_cast<string*>(pResourceData);
	_name = make_shared<string>(name);

	assert(pData && pPath);

	Json::Value json;
	Json::Reader reader;
	ifstream stream(*pPath);
	reader.parse(stream, json, false);

	stream.close();

	int meshCount = json.size();

	_meshes.reset(new MeshVec);
	_meshes->resize(meshCount);

	shared_ptr<StaticMesh> newMesh;
	int i = 0;
	for (auto mesh : json)
	{
		string meshName = *pPath + "_" + to_string(i);
		mesh["name"] = meshName;
		newMesh = mesh["is_skinned"].asBool() ?
				Resources->CreateFromData<SkinnedMesh>(&mesh, meshName) :
				Resources->CreateFromData<StaticMesh>(&mesh, meshName);
		
		_meshes->data()[i++] = newMesh;
	}

	return true;
}

void Mesh::Destroy()
{
	_meshes.reset();
}
