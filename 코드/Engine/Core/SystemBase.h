#pragma once

#include "..\stdafx.h"

class GameObject;
class SceneManager;

class SystemBase abstract
{
protected:
	SceneManager* _pSceneManager;

protected:
	virtual bool filter(const shared_ptr<GameObject> gameObject) abstract;

public:
	SystemBase(SceneManager* pSceneManager) : _pSceneManager(pSceneManager) {}
	virtual ~SystemBase() {}

	virtual void Update() abstract;
	
};