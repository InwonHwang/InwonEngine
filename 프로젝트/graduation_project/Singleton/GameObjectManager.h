#pragma once

#include "..\Core\Core.h"

class GameObject;

class GameObjectManager : public SingletonBase<GameObjectManager>
{
private:
	shared_ptr<list<shared_ptr<GameObject>>> _actived;
	shared_ptr<list<shared_ptr<GameObject>>> _deactived;
 
public:
	GameObjectManager();
	~GameObjectManager();
	
	bool Init() override;
	void Release() override;

	template<typename T>
	shared_ptr<vector<shared_ptr<GameObject>>> FindObjectsOfType();

	void Add(shared_ptr<GameObject> gameObject);
	void Destroy(shared_ptr<GameObject>& gameObject);
	shared_ptr<GameObject> Find(const string& name);
};

template<typename T>
inline shared_ptr<vector<shared_ptr<GameObject>>> GameObjectManager::FindObjectsOfType()
{
	shared_ptr<vector<shared_ptr<GameObject>>> ret(new vector<shared_ptr<GameObject>>());
	for (auto gameObject : *_actived)
	{
		if (!gameObject->GetComponent<T>()) continue;			

		ret->push_back(gameObject);			
	}

	return ret;
}