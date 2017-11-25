#include "Scene.h"
#include "..\Component\Component.h"
#include "..\Singleton\Singleton.h"

Scene::Scene()
	: ResourceBase(),
	_root(nullptr)
{
	_root = Resources->Create<GameObject>("GameObject");
}

Scene::~Scene()
{		
}

bool Scene::Create(void* pData, void* pResourceData, const string& name)
{
	*_name = name;

	string* pPath = static_cast<string*>(pResourceData);

	Json::Value json;
	if (!JsonUtility::ReadFromFile(*pPath, json))
		return false;

	FromJson(json);

	return true;
}

void Scene::Destroy()
{
}

void Scene::ToJson(Json::Value& json)
{
	_root->ToJson(json);

	if (_skyBox)
		json["skybox"] = *_skyBox->GetName();
}

void Scene::FromJson(const Json::Value& json)
{
	_root->FromJson(json);

	if (!json["skybox"].isNull())
		_skyBox = Resources->Create<SkyBox>(json["skybox"].asString());
}

void Scene::Export(const string& path)
{
	Json::Value json;
	ToJson(json);
	JsonUtility::WriteToFile(path, json);
}

shared_ptr<GameObject> Scene::CreateGameObject() const
{
	auto newGameObject = Resources->Create<GameObject>("GameObject");
	newGameObject->GetComponent<Transform>()->SetParent(_root->GetComponent<Transform>());

	return newGameObject;
}