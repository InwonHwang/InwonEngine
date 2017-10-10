//#include "SceneManager.h"
//#include "Scene.h"
//
//SceneManager::SceneManager()
//{
//	_pCurScene = nullptr;
//	_pSceneList.reset(new SceneList);
//}
//
//SceneManager::~SceneManager()
//{
//	_pCurScene.reset();
//	_pSceneList.reset();
//}
//
//void SceneManager::AddScene(const shared_ptr<Scene> pScene)
//{
//	if (!_pCurScene)
//		_pCurScene = pScene;
//
//	_pSceneList->push_back(pScene);
//}
//
//void SceneManager::Clear()
//{
//
//	_pSceneList->clear();
//}
