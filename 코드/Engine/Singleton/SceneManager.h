#pragma once

#include "..\Core\Core.h"

class Scene;

class SceneManager : public SingletonBase<SceneManager>
{
	typedef map<string, shared_ptr<Scene>> SceneMap;

private:
	shared_ptr<Scene> _currentScene;
	unique_ptr<SceneMap> _scenes;

public:
	SceneManager();
	~SceneManager();	

	bool Init() override;
	void Release() override;

	shared_ptr<Scene> GetCurrentScene() const;
	void Load(const string& name);
	void Load(const string& name, const shared_ptr<Scene> scene);	
};