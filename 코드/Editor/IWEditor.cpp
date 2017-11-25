#include "IWEditor.h"

#include "Window/Window.h"
#include "IWEditorRenderSystem.h"

// ImGui Win32 + DirectX9 binding
// In this binding, ImTextureID is used to store a 'LPDIRECT3DTEXTURE9' texture identifier. Read the FAQ about ImTextureID in imgui.cpp.

// You can copy and use unmodified imgui_impl_* files in your project. See main.cpp for an example of using this.
// If you use this binding you'll need to call 4 functions: ImGui_ImplXXXX_Init(), ImGui_ImplXXXX_NewFrame(), ImGui::Render() and ImGui_ImplXXXX_Shutdown().
// If you are new to ImGui, see examples/README.txt and documentation at the top of imgui.cpp.
// https://github.com/ocornut/imgui

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

extern IWEditor editor;

shared_ptr<GameObject> IWEditor::SelectedObject;
string IWEditor::SelectedDirectory;
string IWEditor::SelectedFile;
static LPDIRECT3DVERTEXBUFFER9  g_pVB = NULL;
static LPDIRECT3DINDEXBUFFER9   g_pIB = NULL;
static int                      g_VertexBufferSize = 5000, g_IndexBufferSize = 10000;

void IWEditor::CreateScene()
{
	Scenes->Load("New Scene", make_shared<Scene>());
	Scenes->GetCurrentScene()->GetRootGameObject()->SetName("New Scene");
	auto camera = IWEditor::CreateCamera();
	camera->SetName("main Camera");
	Camera::mainCamera = camera->GetComponent<Camera>();
}

shared_ptr<GameObject> IWEditor::CreateCamera()
{
	auto newObject = GameObject::Create();
	newObject->AddComponent<Camera>();
	newObject->GetComponent<Transform>()->SetLocalPosition(Vector3(0, 0, -10));

	return newObject;
}

shared_ptr<GameObject> IWEditor::CreateGameObject()
{
	return GameObject::Create();
}

IWEditor::IWEditor()
	: _windowContainer(nullptr),
	_hInstance(0),
	_hWnd(0),
	_time(0),
	_ticksPerSecond(0),
	_ppVB(0),
	_ppIB(0),
	_pTexture(0),
	_vertexBufferSize(5000),
	_indexBufferSize(10000)
{
}


IWEditor::~IWEditor()
{
}

LRESULT WINAPI IWEditor::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (IWEditor::ImGuiWndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
		if (D3D9 && D3D9Device && wParam != SIZE_MINIMIZED)
		{
		editor.ImGuiInvalidateDeviceObjects();
		D3D9->SetBackBufferSize(LOWORD(lParam), HIWORD(lParam));
		editor.ImGuiCreateDeviceObjects();
		}
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

IMGUI_API LRESULT IWEditor::ImGuiWndProcHandler(HWND, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ImGuiIO& io = ImGui::GetIO();
	switch (msg)
	{
	case WM_LBUTTONDOWN:
		io.MouseDown[0] = true;
		return true;
	case WM_LBUTTONUP:
		io.MouseDown[0] = false;
		return true;
	case WM_RBUTTONDOWN:
		io.MouseDown[1] = true;
		return true;
	case WM_RBUTTONUP:
		io.MouseDown[1] = false;
		return true;
	case WM_MBUTTONDOWN:
		io.MouseDown[2] = true;
		return true;
	case WM_MBUTTONUP:
		io.MouseDown[2] = false;
		return true;
	case WM_MOUSEWHEEL:
		io.MouseWheel += GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? +1.0f : -1.0f;
		return true;
	case WM_MOUSEMOVE:
		io.MousePos.x = (signed short)(lParam);
		io.MousePos.y = (signed short)(lParam >> 16);
		return true;
	case WM_KEYDOWN:
		if (wParam < 256)
			io.KeysDown[wParam] = 1;
		return true;
	case WM_KEYUP:
		if (wParam < 256)
			io.KeysDown[wParam] = 0;
		return true;
	case WM_CHAR:
		// You can also use ToAscii()+GetKeyboardState() to retrieve characters.
		if (wParam > 0 && wParam < 0x10000)
			io.AddInputCharacter((unsigned short)wParam);
		return true;
	}
	return 0;
}

// This is the main rendering function that you have to implement and provide to ImGui (via setting up 'RenderDrawListsFn' in the ImGuiIO structure)
// If text or lines are blurry when integrating ImGui in your engine:
// - in your Render function, try translating your projection matrix by (0.5f,0.5f) or (0.375f,0.375f)
void IWEditor::ImGuiRenderDrawLists(ImDrawData* draw_data)
{
	// Avoid rendering when minimized
	ImGuiIO& io = ImGui::GetIO();
	if (io.DisplaySize.x <= 0.0f || io.DisplaySize.y <= 0.0f)
		return;

	// Create and grow buffers if needed
	if (!g_pVB || g_VertexBufferSize < draw_data->TotalVtxCount)
	{
		if (g_pVB) { g_pVB->Release(); g_pVB = NULL; }
		g_VertexBufferSize = draw_data->TotalVtxCount + 5000;
		if (D3D9Device->CreateVertexBuffer(g_VertexBufferSize * sizeof(CUSTOMVERTEX), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, CUSTOMVERTEX::FVF, D3DPOOL_DEFAULT, &g_pVB, NULL) < 0)
			return;
	}
	if (!g_pIB || g_IndexBufferSize < draw_data->TotalIdxCount)
	{
		if (g_pIB) { g_pIB->Release(); g_pIB = NULL; }
		g_IndexBufferSize = draw_data->TotalIdxCount + 10000;
		if (D3D9Device->CreateIndexBuffer(g_IndexBufferSize * sizeof(ImDrawIdx), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, sizeof(ImDrawIdx) == 2 ? D3DFMT_INDEX16 : D3DFMT_INDEX32, D3DPOOL_DEFAULT, &g_pIB, NULL) < 0)
			return;
	}

	// Backup the DX9 state
	IDirect3DStateBlock9* d3d9_state_block = NULL;
	if (D3D9Device->CreateStateBlock(D3DSBT_ALL, &d3d9_state_block) < 0)
		return;

	// Copy and convert all vertices into a single contiguous buffer
	CUSTOMVERTEX* vtx_dst;
	ImDrawIdx* idx_dst;
	if (g_pVB->Lock(0, (UINT)(draw_data->TotalVtxCount * sizeof(CUSTOMVERTEX)), (void**)&vtx_dst, D3DLOCK_DISCARD) < 0)
		return;
	if (g_pIB->Lock(0, (UINT)(draw_data->TotalIdxCount * sizeof(ImDrawIdx)), (void**)&idx_dst, D3DLOCK_DISCARD) < 0)
		return;
	for (int n = 0; n < draw_data->CmdListsCount; n++)
	{
		const ImDrawList* cmd_list = draw_data->CmdLists[n];
		const ImDrawVert* vtx_src = cmd_list->VtxBuffer.Data;
		for (int i = 0; i < cmd_list->VtxBuffer.Size; i++)
		{
			vtx_dst->pos[0] = vtx_src->pos.x;
			vtx_dst->pos[1] = vtx_src->pos.y;
			vtx_dst->pos[2] = 0.0f;
			vtx_dst->col = (vtx_src->col & 0xFF00FF00) | ((vtx_src->col & 0xFF0000) >> 16) | ((vtx_src->col & 0xFF) << 16);     // RGBA --> ARGB for DirectX9
			vtx_dst->uv[0] = vtx_src->uv.x;
			vtx_dst->uv[1] = vtx_src->uv.y;
			vtx_dst++;
			vtx_src++;
		}
		memcpy(idx_dst, cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));
		idx_dst += cmd_list->IdxBuffer.Size;
	}
	g_pVB->Unlock();
	g_pIB->Unlock();
	D3D9Device->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX));
	D3D9Device->SetIndices(g_pIB);
	D3D9Device->SetFVF(CUSTOMVERTEX::FVF);

	// Setup viewport
	D3DVIEWPORT9 vp;
	vp.X = vp.Y = 0;
	vp.Width = (DWORD)io.DisplaySize.x;
	vp.Height = (DWORD)io.DisplaySize.y;
	vp.MinZ = 0.0f;
	vp.MaxZ = 1.0f;
	D3D9Device->SetViewport(&vp);

	// Setup render state: fixed-pipeline, alpha-blending, no face culling, no depth testing
	D3D9Device->SetPixelShader(NULL);
	D3D9Device->SetVertexShader(NULL);
	D3D9Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	D3D9Device->SetRenderState(D3DRS_LIGHTING, false);
	D3D9Device->SetRenderState(D3DRS_ZENABLE, false);
	D3D9Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	D3D9Device->SetRenderState(D3DRS_ALPHATESTENABLE, false);
	D3D9Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	D3D9Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	D3D9Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	D3D9Device->SetRenderState(D3DRS_SCISSORTESTENABLE, true);
	D3D9Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	D3D9Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	D3D9Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	D3D9Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	D3D9Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	D3D9Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	D3D9Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	D3D9Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	// Setup orthographic projection matrix
	// Being agnostic of whether <d3dx9.h> or <DirectXMath.h> can be used, we aren't relying on D3DXMatrixIdentity()/D3DXMatrixOrthoOffCenterLH() or DirectX::XMMatrixIdentity()/DirectX::XMMatrixOrthographicOffCenterLH()
	{
		const float L = 0.5f, R = io.DisplaySize.x + 0.5f, T = 0.5f, B = io.DisplaySize.y + 0.5f;
		D3DMATRIX mat_identity = { { 1.0f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 0.0f, 1.0f } };
		D3DMATRIX mat_projection =
		{
			2.0f / (R - L),   0.0f,         0.0f,  0.0f,
			0.0f,         2.0f / (T - B),   0.0f,  0.0f,
			0.0f,         0.0f,         0.5f,  0.0f,
			(L + R) / (L - R),  (T + B) / (B - T),  0.5f,  1.0f,
		};
		D3D9Device->SetTransform(D3DTS_WORLD, &mat_identity);
		D3D9Device->SetTransform(D3DTS_VIEW, &mat_identity);
		D3D9Device->SetTransform(D3DTS_PROJECTION, &mat_projection);
	}

	// Render command lists
	int vtx_offset = 0;
	int idx_offset = 0;
	for (int n = 0; n < draw_data->CmdListsCount; n++)
	{
		const ImDrawList* cmd_list = draw_data->CmdLists[n];
		for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
		{
			const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
			if (pcmd->UserCallback)
			{
				pcmd->UserCallback(cmd_list, pcmd);
			}
			else
			{
				const RECT r = { (LONG)pcmd->ClipRect.x, (LONG)pcmd->ClipRect.y, (LONG)pcmd->ClipRect.z, (LONG)pcmd->ClipRect.w };
				D3D9Device->SetTexture(0, (LPDIRECT3DTEXTURE9)pcmd->TextureId);
				D3D9Device->SetScissorRect(&r);
				D3D9Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, vtx_offset, 0, (UINT)cmd_list->VtxBuffer.Size, idx_offset, pcmd->ElemCount / 3);
			}
			idx_offset += pcmd->ElemCount;
		}
		vtx_offset += cmd_list->VtxBuffer.Size;
	}

	// Restore the DX9 state
	d3d9_state_block->Apply();
	d3d9_state_block->Release();
}

bool IWEditor::ImGuiInit()
{
	if (!QueryPerformanceFrequency((LARGE_INTEGER *)&_ticksPerSecond))
		return false;
	if (!QueryPerformanceCounter((LARGE_INTEGER *)&_time))
		return false;

	ImGuiIO& io = ImGui::GetIO();
	io.KeyMap[ImGuiKey_Tab] = VK_TAB;                       // Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array that we will update during the application lifetime.
	io.KeyMap[ImGuiKey_LeftArrow] = VK_LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = VK_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow] = VK_UP;
	io.KeyMap[ImGuiKey_DownArrow] = VK_DOWN;
	io.KeyMap[ImGuiKey_PageUp] = VK_PRIOR;
	io.KeyMap[ImGuiKey_PageDown] = VK_NEXT;
	io.KeyMap[ImGuiKey_Home] = VK_HOME;
	io.KeyMap[ImGuiKey_End] = VK_END;
	io.KeyMap[ImGuiKey_Delete] = VK_DELETE;
	io.KeyMap[ImGuiKey_Backspace] = VK_BACK;
	io.KeyMap[ImGuiKey_Enter] = VK_RETURN;
	io.KeyMap[ImGuiKey_Escape] = VK_ESCAPE;
	io.KeyMap[ImGuiKey_A] = 'A';
	io.KeyMap[ImGuiKey_C] = 'C';
	io.KeyMap[ImGuiKey_V] = 'V';
	io.KeyMap[ImGuiKey_X] = 'X';
	io.KeyMap[ImGuiKey_Y] = 'Y';
	io.KeyMap[ImGuiKey_Z] = 'Z';

	io.RenderDrawListsFn = ImGuiRenderDrawLists;   // Alternatively you can set this to NULL and call ImGui::GetDrawData() after ImGui::Render() to get the same ImDrawData pointer.
	io.ImeWindowHandle = _hWnd;

	return true;
}

void IWEditor::ImGuiShutdown()
{
	ImGuiInvalidateDeviceObjects();
	ImGui::Shutdown();
	_hWnd = 0;
}

bool IWEditor::ImGuiCreateFontsTexture()
{
	// Build texture atlas
	ImGuiIO& io = ImGui::GetIO();
	unsigned char* pixels;
	int width, height, bytes_per_pixel;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height, &bytes_per_pixel);

	// Upload texture to graphics system
	_pTexture = NULL;
	if (D3D9Device->CreateTexture(width, height, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &_pTexture, NULL) < 0)
		return false;
	D3DLOCKED_RECT tex_locked_rect;
	if (_pTexture->LockRect(0, &tex_locked_rect, NULL, 0) != D3D_OK)
		return false;
	for (int y = 0; y < height; y++)
		memcpy((unsigned char *)tex_locked_rect.pBits + tex_locked_rect.Pitch * y, pixels + (width * bytes_per_pixel) * y, (width * bytes_per_pixel));
	_pTexture->UnlockRect(0);

	// Store our identifier
	io.Fonts->TexID = (void *)_pTexture;

	return true;
}

bool IWEditor::ImGuiCreateDeviceObjects()
{
	if (!D3D9Device) return false;
		
	if (!ImGuiCreateFontsTexture()) return false;
		
	return true;
}

void IWEditor::ImGuiInvalidateDeviceObjects()
{
	if (!D3D9Device) return;
		
	if (g_pVB)
	{
		g_pVB->Release();
		g_pVB = NULL;
	}
	if (g_pIB)
	{
		g_pIB->Release();
		g_pIB = NULL;
	}

	// At this point note that we set ImGui::GetIO().Fonts->TexID to be == _pTexture, so clear both.
	ImGuiIO& io = ImGui::GetIO();
	IM_ASSERT(_pTexture == io.Fonts->TexID);
	if (_pTexture) _pTexture->Release();
		
	_pTexture = NULL;
	io.Fonts->TexID = NULL;
}

void IWEditor::ImGuiNewFrame()
{
	if (!_pTexture) ImGuiCreateDeviceObjects();
		

	ImGuiIO& io = ImGui::GetIO();

	// Setup display size (every frame to accommodate for window resizing)
	RECT rect;
	GetClientRect(_hWnd, &rect);
	io.DisplaySize = ImVec2((float)(rect.right - rect.left), (float)(rect.bottom - rect.top));

	// Setup time step
	INT64 current_time;
	QueryPerformanceCounter((LARGE_INTEGER *)&current_time);
	io.DeltaTime = (float)(current_time - _time) / _ticksPerSecond;
	_time = current_time;

	// Read keyboard modifiers inputs
	io.KeyCtrl = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
	io.KeyShift = (GetKeyState(VK_SHIFT) & 0x8000) != 0;
	io.KeyAlt = (GetKeyState(VK_MENU) & 0x8000) != 0;
	io.KeySuper = false;
	// io.KeysDown : filled by WM_KEYDOWN/WM_KEYUP events
	// io.MousePos : filled by WM_MOUSEMOVE events
	// io.MouseDown : filled by WM_*BUTTON* events
	// io.MouseWheel : filled by WM_MOUSEWHEEL events

	// Set OS mouse position if requested last frame by io.WantMoveMouse flag (used when io.NavMovesTrue is enabled by user and using directional navigation)
	if (io.WantMoveMouse)
	{
		POINT pos = { (int)io.MousePos.x, (int)io.MousePos.y };
		ClientToScreen(_hWnd, &pos);
		SetCursorPos(pos.x, pos.y);
	}

	// Hide OS mouse cursor if ImGui is drawing it
	if (io.MouseDrawCursor)
		SetCursor(NULL);

	// Start the frame
	ImGui::NewFrame();
}

bool IWEditor::Init()
{
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, LoadCursor(NULL, IDC_ARROW), NULL, NULL, "IWEditor", NULL };
	RegisterClassEx(&wc);
	_hWnd = CreateWindow("IWEditor", "IWEditor", WS_OVERLAPPEDWINDOW, 50, 50, 1600, 900, NULL, NULL, wc.hInstance, NULL);
	_hInstance = wc.hInstance;
	SetActiveWindow(_hWnd);

	D3D9->Init();
	Physics->Init();
	Resources->Init();
	Scenes->Init();

	ImGuiInit();
	
	_windowContainer = make_unique<WindowManager>("IWEditor", ImVec2(0,0), ImGui::GetIO().DisplaySize);

	_transformSystem = make_unique<TransformSystem>(D3D9);
	_materialSystem = make_unique<MaterialSystem>(D3D9);
	_renderSystem = make_unique<IWEditorRenderSystem>(D3D9);

	// Load Fonts
	// (there is a default font, this is only if you want to change it. see extra_fonts/README.txt for more details)
	//ImGuiIO& io = ImGui::GetIO();
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("../../extra_fonts/Cousine-Regular.ttf", 15.0f);
	//io.Fonts->AddFontFromFileTTF("../../extra_fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../extra_fonts/Roboto-Medium.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyTiny.ttf", 10.0f);
	//io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());

	return true;
}

void IWEditor::Update()
{
	bool show_test_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	ShowWindow(_hWnd, SW_SHOWDEFAULT);
	UpdateWindow(_hWnd);
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			continue;
		}
		else
		{
			ImGuiNewFrame();

			_windowContainer->Update();

			_transformSystem->Update();
			_materialSystem->Update();
			_renderSystem->Update();

			auto mainCamera = Camera::mainCamera;

			if (mainCamera)
			{
				const float moveSpeed = 1.0f;
				const float rotationSpeed = 1.0f;
				auto transform = mainCamera->GetComponent<Transform>();

				auto camPos = transform->GetLocalPosition();
				if (Input->GetKeyDown(InputManager::KeyCode::eKey_W)) transform->SetLocalPosition(*camPos + Vector3(0, 0, moveSpeed));
				if (Input->GetKeyDown(InputManager::KeyCode::eKey_S)) transform->SetLocalPosition(*camPos + Vector3(0, 0, -moveSpeed));
				if (Input->GetKeyDown(InputManager::KeyCode::eKey_A)) transform->SetLocalPosition(*camPos + Vector3(moveSpeed, 0, 0));
				if (Input->GetKeyDown(InputManager::KeyCode::eKey_D)) transform->SetLocalPosition(*camPos + Vector3(-moveSpeed, 0, 0));
				if (Input->GetKeyDown(InputManager::KeyCode::eKey_R)) transform->SetLocalPosition(*camPos + Vector3(0, moveSpeed, 0));
				if (Input->GetKeyDown(InputManager::KeyCode::eKey_F)) transform->SetLocalPosition(*camPos + Vector3(0, -moveSpeed, 0));


				Vector3 resultAngle;
				Vector3 eulerAngle = Quaternion::ToEulerAngle(*transform->GetRotation());

				if (Input->GetKeyDown(InputManager::KeyCode::eKey_I))
				{
					resultAngle = eulerAngle + Vector3(-rotationSpeed, 0, 0);
					transform->SetLocalRotation(Quaternion::Euler(resultAngle.x, resultAngle.y, resultAngle.z));
				}
				if (Input->GetKeyDown(InputManager::KeyCode::eKey_K))
				{
					resultAngle = eulerAngle + Vector3(rotationSpeed, 0, 0);
					transform->SetLocalRotation(Quaternion::Euler(resultAngle.x, resultAngle.y, resultAngle.z));
				}
				if (Input->GetKeyDown(InputManager::KeyCode::eKey_J))
				{
					resultAngle = eulerAngle + Vector3(0, rotationSpeed, 0);
					transform->SetLocalRotation(Quaternion::Euler(resultAngle.x, resultAngle.y, resultAngle.z));
				}
				if (Input->GetKeyDown(InputManager::KeyCode::eKey_L))
				{
					resultAngle = eulerAngle + Vector3(0, -rotationSpeed, 0);
					transform->SetLocalRotation(Quaternion::Euler(resultAngle.x, resultAngle.y, resultAngle.z));
				}
			}
		}
	}
}

void IWEditor::Release()
{
	ImGuiShutdown();
	Resources->Release();
	Scenes->Release();
	D3D9->Release();
	Physics->Release();
	UnregisterClass("IWEditor", _hInstance);
}

const shared_ptr<WindowBase> IWEditor::GetWindow(const string& name) const
{
	return _windowContainer->Get(name);
}