#include "Application.h"
#include "..\Component\Component.h"
#include "..\Physics\Physics.h"
#include "..\Resource\Resource.h"
#include "..\Graphics\Graphics.h"
#include "..\Singleton\Singleton.h"

LRESULT CALLBACK Application::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
	} break;

	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

bool Application::Init()
{
	return true;
}

bool Application::Init(HINSTANCE hInstance, int nCmdShow)
{
	
	_hInstance = hInstance;
	_nCmdShow = nCmdShow;
	_screenWidth = 800;
	_screenHeight = 600;

	HWND hWnd;
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)Application::WindowProc;
	wc.hInstance = _hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = "WindowClass1";

	RegisterClassEx(&wc);

	hWnd = CreateWindowEx(NULL,
						"WindowClass1",
						"Our Direct3D Program",
						WS_OVERLAPPEDWINDOW,
						0,
						0,
						_screenWidth,
						_screenHeight,
						NULL,
						NULL,
						_hInstance,
						NULL);

	ShowWindow(hWnd, _nCmdShow);

	SetActiveWindow(hWnd);

	ZeroMemory(&_msg, sizeof(MSG));
	
	D3D9->Init();
	Physics->Init();
	AppTimer->Init();
	Resources->Init();
	Input->Init();
	Scenes->Init();

	_behaviourSystem = make_unique<BehaviourSystem>();
	_renderSystem = make_unique<RenderSystem>(D3D9);
	_transformSystem = make_unique<TransformSystem>(D3D9);
	_physicsSystem = make_unique<PhysicsSystem>(Physics);
	_animationSystem = make_unique<AnimationSystem>();
	_materialSystem = make_unique<MaterialSystem>(D3D9);
	
	
	REGISTER_TYPE("Trasnform", []() -> void* { return new Transform(nullptr); }, [](void* p) { SafeDelete<Transform>(p); });
	REGISTER_TYPE("MeshRenderer", []() -> void* { return new MeshRenderer(nullptr); }, [](void* p) { SafeDelete<MeshRenderer>(p); });
	REGISTER_TYPE("SkinnedMeshRenderer", []() -> void* { return new SkinnedMeshRenderer(nullptr); }, [](void* p) { SafeDelete<SkinnedMeshRenderer>(p); });
	REGISTER_TYPE("Animator", []() -> void* { return new Animator(nullptr); }, [](void* p) { SafeDelete<Animator>(p); });

	return true;
}

void Application::Update()
{
	while (_msg.message != WM_QUIT)
	{
		AppTimer->Update();
		if (PeekMessage(&_msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&_msg);
			DispatchMessage(&_msg);
		}
		else
		{	
			_behaviourSystem->Update();
			_animationSystem->Update();
			_transformSystem->Update();
			_physicsSystem->Update();
			_materialSystem->Update();
			_renderSystem->Update();
		}
	}
}

void Application::Release()
{
	Scenes->Release();
	Resources->Release();
	D3D9->Release();
	Physics->Release();
	AppTimer->Release();
	Input->Release();
}