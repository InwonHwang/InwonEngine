#pragma once

#include "..\Core\Core.h"
#include "..\System\System.h"

class Application : public SingletonBase<Application>
{
private:
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	unique_ptr<BehaviourSystem>	_behaviourSystem;
	unique_ptr<PhysicsSystem>	_physicsSystem;
	unique_ptr<RenderSystem>	_renderSystem;
	unique_ptr<TransformSystem>	_transformSystem;
	unique_ptr<AnimationSystem>	_animationSystem;
	unique_ptr<MaterialSystem>	_materialSystem;

public:
	Application() {}
	~Application() {}

	bool Init(HINSTANCE hInstance, int nCmdShow);
	bool Init() override;
	void Release() override;
	void Update();

private:
	HINSTANCE	_hInstance;
	int			_nCmdShow;
	int			_screenWidth;
	int			_screenHeight;
	MSG			_msg;
};