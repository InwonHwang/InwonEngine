#include "GameObjectManager.h"
#include "..\Resource\Resource.h"
#include "..\Component\Component.h"

GameObjectManager::GameObjectManager()
{
}

GameObjectManager::~GameObjectManager()
{
}

bool GameObjectManager::Init()
{
	_actived = make_shared<list<shared_ptr<GameObject>>>();
	_deactived = make_shared<list<shared_ptr<GameObject>>>();

	return true;
}

void GameObjectManager::Release()
{
	for (auto gameObject : *_actived) {
		gameObject->Destroy();
		gameObject.reset();
	}

	for (auto gameObject : *_deactived) {
		gameObject->Destroy();
		gameObject.reset();
	}

	_actived.reset();
	_deactived.reset();
}

void GameObjectManager::Add(shared_ptr<GameObject> gameObject)
{
	_actived->push_back(gameObject);
}

void GameObjectManager::Destroy(shared_ptr<GameObject>& gameObject)
{
	shared_ptr<GameObject> pTempGameObject;
	shared_ptr<Transform> pTempTransform = gameObject->GetComponent<Transform>();

	size_t childCount = pTempTransform->GetChildCount();
	for (size_t i = 0; i < childCount; ++i)
	{
		pTempGameObject = pTempTransform->GetChild(i)->GetGameObject();
		Destroy(pTempGameObject);
	}

	_actived->remove(gameObject);
	_deactived->remove(gameObject);
	gameObject.reset();
}

shared_ptr<GameObject> GameObjectManager::Find(const string& name)
{
	for each(auto gameObject in *_actived)
	{
		if (gameObject->GetName()->compare(name) == 0)
			return gameObject;
	}

	return nullptr;
}