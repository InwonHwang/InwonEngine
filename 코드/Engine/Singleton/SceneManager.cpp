#include "SceneManager.h"
#include "..\Resource\Resource.h"
#include "ResourceManager.h"

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

bool SceneManager::Init()
{
	_scenes = make_unique<SceneMap>();

	return true;
}

void SceneManager::Release()
{

}

shared_ptr<Scene> SceneManager::GetCurrentScene() const
{
	return _currentScene;
}

void SceneManager::Load(const string& name)
{
	auto newScene = ResourceManager::Instance()->Create<Scene>(name);
	Load(name, newScene);
}

void SceneManager::Load(const string& name, const shared_ptr<Scene> scene)
{
	_scenes->insert(make_pair(name, scene));
	_currentScene = scene;
}