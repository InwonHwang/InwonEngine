#include "GameObject.h"
#include "..\Component\Component.h"
#include "..\Singleton\Singleton.h"



shared_ptr<GameObject> GameObject::Instantiate(shared_ptr<GameObject> prefab)
{
	//shared_ptr<GameObject> gameObject = pModelData->InstantiateRecursively();
	return nullptr;
}

shared_ptr<GameObject> GameObject::Create()
{
	auto newGameObject = Resources->Create<GameObject>("GameObject");
	GameObjectManager::Instance()->Add(newGameObject);

	return newGameObject;
}

void GameObject::Destroy(shared_ptr<GameObject>& gameObject)
{
	/*GameObjectManager::Instance()->Destroy(gameObject);
	shared_ptr<GameObject> pTempGameObject;
	shared_ptr<Transform> pTempTransform = gameObject->GetComponent<Transform>();

	size_t childCount = pTempTransform->GetChildCount();
	for (size_t i = 0; i < childCount; ++i)
	{
	pTempGameObject = pTempTransform->GetChild(i)->GetGameObject();
	Destroy(pTempGameObject);
	}

	gameObject.reset();*/
}

shared_ptr<GameObject> GameObject::Find(const string& name)
{
	return GameObjectManager::Instance()->Find(name);
}

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

bool GameObject::Create(void* pData, void* pResourceData, const string& name)
{
	_components = make_unique<ComponentVec>();
	_name = make_shared<string>(name);

	if (name.compare("GameObject") == 0) return true;

	string* pPath = static_cast<string*>(pResourceData);

	Json::Value json;
	Json::Reader reader;
	ifstream stream(*pPath);
	reader.parse(stream, json, false);

	stream.close();

	_name = make_shared<string>(json["name"].asString());
	vector<shared_ptr<GameObject>> gameObjects({ shared_from_this() });
	vector<int> parentIndices({ -1 });
	AddComponent<Transform>();

	int i = 0;
	for (auto gameObjectInfo : json["GameObjects"])
	{
		if (i++ == 0) continue;

		auto newGameObject = Resources->Create<GameObject>("GameObject");
		newGameObject->SetName(gameObjectInfo["name"].asString());

		for (auto key : gameObjectInfo["Components"].getMemberNames())
		{
			//newGameObject->AddComponent(key.c_str());			
			if (key.compare("Transform") == 0)
			{
				auto transform = newGameObject->AddComponent<Transform>();
				Json::Value transformInfo = json["Transforms"][gameObjectInfo["Components"][key].asInt()];

				transform->SetLocalScale(Vector3(transformInfo["scale"]["x"].asFloat(),
					transformInfo["scale"]["y"].asFloat(),
					transformInfo["scale"]["z"].asFloat()));

				transform->SetLocalRotation(Quaternion(transformInfo["rotation"]["x"].asFloat(),
					transformInfo["rotation"]["y"].asFloat(),
					transformInfo["rotation"]["z"].asFloat(),
					transformInfo["rotation"]["w"].asFloat()));

				transform->SetLocalPosition(Vector3(transformInfo["position"]["x"].asFloat(),
					transformInfo["position"]["y"].asFloat(),
					transformInfo["position"]["z"].asFloat()));

				parentIndices.push_back(transformInfo["parent_index"].asInt());
			}
			else if (key.compare("MeshRenderer") == 0)
			{
				auto meshRenderer = newGameObject->AddComponent<MeshRenderer>();
				Json::Value meshRendererInfo = json["MeshRenderers"][gameObjectInfo["Components"][key].asInt()];
				Json::Value materialInfos = meshRendererInfo["Materials"];

				meshRenderer->SetMaterialCount(materialInfos.size());

				int i = 0;
				for (auto materialInfo : materialInfos)
				{
					auto material = Resources->Create<Material>("Assets\\material\\" + materialInfo.asString() + ".material");
					meshRenderer->SetMaterial(i++, material);
				}

				auto mesh = Resources->Create<Mesh>("Assets\\mesh\\" + meshRendererInfo["Mesh"].asString() + ".mesh");
				meshRenderer->SetMesh(mesh);
			}
			else if (key.compare("SkinnedMeshRenderer") == 0)
			{

			}
		}

		gameObjects.push_back(newGameObject);
	}


	//i = 0;
	//for (auto gameObject : json["GameObjects"])
	//{
	//	if (i == 0) continue;

	//	//for (auto key : gameObject["Components"].getMemberNames())
	//	//{
	//	//	//newGameObject->AddComponent(key);

	//	//	if (key.compare("Transform") == 0)
	//	//	{
	//	//		Json::Value transform = json["Transforms"][gameObject["Components"][key].asInt()];
	//	//		int parentIndex = transform["parent_index"].asInt();
	//	//	}
	//	//}
	//}

	

	return true;
}

void GameObject::Destroy()
{
	for (auto c : *_components)
		c->Destroy();
}