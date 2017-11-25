#include "GameObject.h"
#include "..\Component\Component.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

shared_ptr<GameObject> GameObject::Instantiate(shared_ptr<GameObject> prefab)
{
	//shared_ptr<GameObject> gameObject = pModelData->InstantiateRecursively();
	return nullptr;
}

shared_ptr<GameObject> GameObject::Create()
{
	return Scenes->GetCurrentScene()->CreateGameObject();
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
	auto ret = Scenes->GetCurrentScene()->GetRootGameObject()->GetComponent<Transform>()->FindChild(name);
	return ret ? ret->GetGameObject() : nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

GameObject::GameObject()
	: ResourceBase(),
	_components(nullptr)
{
	_components = make_unique<ComponentVec>();
}

GameObject::~GameObject()
{
}

void GameObject::getJsonRecursively(const shared_ptr<GameObject> gameObject, Json::Value& json, int index, int parentIndex)
{
	Json::Value gameObjectInfo;
	gameObjectInfo["name"] = *gameObject->GetName();
	gameObjectInfo["parent_index"] = parentIndex;

	for (auto component : *gameObject->_components)
	{
		auto componentName = split(typeid(*component).name(), ' ')->data()[1];

		Json::Value componentInfo;
		component->ToJson(componentInfo);
		gameObjectInfo["components"][componentName] = json[componentName].size();
		json[componentName].append(componentInfo);
	}

	json["GameObject"].append(gameObjectInfo);

	auto transform = gameObject->GetComponent<Transform>();

	for (int i = 0; i < transform->GetChildCount(); ++i)
		getJsonRecursively(transform->GetChild(i)->GetGameObject(), json, json["GameObject"].size(), index);
}

void GameObject::createCloneRecursively(const shared_ptr<GameObject> parent, const shared_ptr<GameObject> child)
{
	shared_ptr<GameObject> newGameObject = Resources->Create<GameObject>("GameObject");

	auto transform = child->GetComponent<Transform>();
	for (int i = 0; i < transform->GetChildCount(); ++i) {
		createCloneRecursively(newGameObject, transform->GetChild(i)->GetGameObject());
	}


	if (parent) newGameObject->GetComponent<Transform>()->SetParent(parent->GetComponent<Transform>());
	newGameObject->_name = make_shared<string>(*_name);

	for (auto component : *_components) {
		auto newComponent = static_pointer_cast<ComponentBase>(component->Clone());
		newGameObject->_components->push_back(newComponent);

		if (typeid(*component) == typeid(Animator)) {
			auto animator = static_pointer_cast<Animator>(component);
			auto bones = animator->GetBones();

			auto newBones = make_shared<vector<shared_ptr<Transform>>>();
			for (auto bone : *bones) {
				bone->GetGameObject()->GetName();
			}
			//본 설정
		}

		if (typeid(*component) == typeid(SkinnedMeshRenderer)) {
			auto skinnedMeshRenderer = static_pointer_cast<SkinnedMeshRenderer>(component);
			//본 설정, RootBone
		}
	}
}


bool GameObject::Create(void* pData, void* pResourceData, const string& name)
{
	*_name = name;

	AddComponent<Transform>();

	if (name.compare("GameObject") == 0) return true;

	string* pPath = static_cast<string*>(pResourceData);

	Json::Value json;
	if (!JsonUtility::ReadFromFile(*pPath, json)) {
		Destroy();
		return false;
	}
	
	FromJson(json);

	return true;
}

void GameObject::Destroy()
{
	for (auto c : *_components)
		c->Destroy();

	_components->clear();
}

shared_ptr<void> GameObject::Clone()
{
	return nullptr;
}

void GameObject::Export(const string& path)
{
	Json::Value json;
	ToJson(json);

	JsonUtility::WriteToFile(path, json);
}

void GameObject::FromJson(const Json::Value& json)
{
	vector<shared_ptr<GameObject>> gameObjects;
	map<string, list<pair<Json::Value, shared_ptr<ComponentBase>>>> componentsMap;

	int i = 0;
	for (auto gameObjectInfo : json["GameObject"])
	{
		auto newGameObject = i++ == 0 ? shared_from_this() : Resources->Create<GameObject>("GameObject");
		newGameObject->SetName(gameObjectInfo["name"].asString());

		for (auto componentName : gameObjectInfo["components"].getMemberNames())
		{
			if (componentsMap.find(componentName) == componentsMap.end())
				componentsMap.insert(make_pair(componentName, list<pair<Json::Value, shared_ptr<ComponentBase>>>()));

			shared_ptr<ComponentBase> component;
			Json::Value info = json[componentName][gameObjectInfo["components"][componentName].asInt()];

			if (componentName.compare("Transform") == 0) component = newGameObject->GetComponent<Transform>();
			if (componentName.compare("Animator") == 0) component = newGameObject->AddComponent<Animator>();
			if (componentName.compare("MeshRenderer") == 0) component = newGameObject->AddComponent<MeshRenderer>();
			if (componentName.compare("SkinnedMeshRenderer") == 0) component = newGameObject->AddComponent<SkinnedMeshRenderer>();
			if (componentName.compare("Camera") == 0) {
				component = newGameObject->AddComponent<Camera>();
				if (!Camera::mainCamera) Camera::mainCamera = static_pointer_cast<Camera>(component);
			}

			componentsMap[componentName].push_back(make_pair(info, component));
		}		

		gameObjects.push_back(newGameObject);
	}

	i = 0;
	for (auto gameObjectInfo : json["GameObject"])
	{
		int parentIndex = gameObjectInfo["parent_index"].asInt();

		if (parentIndex == -1) { i++; continue; }

		gameObjects[i++]->GetComponent<Transform>()->SetParent(gameObjects[parentIndex]->GetComponent<Transform>());
	}

	for (auto components : componentsMap)
	{
		for (auto pair : components.second) {

			auto info = pair.first;
			auto component = pair.second;

			if(!info.isNull()) component->FromJson(info);

			if (components.first.compare("SkinnedMeshRenderer") != 0) continue;

			auto rootBone = GetComponent<Transform>()->FindChild(info["root_bone"].asString());
			auto bones = make_shared<vector<shared_ptr<Transform>>>();

			for (auto boneName : info["bones"]) {
				auto bone = rootBone->GetComponent<Transform>()->FindChild(boneName.asString());
				bones->push_back(bone);
			}

			auto skinnedMeshRenderer = static_pointer_cast<SkinnedMeshRenderer>(component);

			skinnedMeshRenderer->SetRootBone(rootBone);
			skinnedMeshRenderer->SetBones(bones);
		}
	}
}

void GameObject::ToJson(Json::Value& json)
{
	getJsonRecursively(shared_from_this(), json, 0, -1);
}