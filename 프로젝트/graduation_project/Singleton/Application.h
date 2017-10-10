#pragma once

#include "..\Core\Core.h"
#include "..\System\System.h"

class Application : public SingletonBase<Application>
{
public:
	static shared_ptr<string> GetDataPath();
private:
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
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

private:

	shared_ptr<BehaviourSystem>		_behaviourSystem;
	shared_ptr<PhysicsSystem>		_physicsSystem;
	shared_ptr<RenderSystem>		_renderSystem;
	shared_ptr<TransformSystem>		_transformSystem;
};