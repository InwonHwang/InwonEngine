#pragma once

#include "stdafx.h"

class WindowManager;
class WindowBase;

struct CUSTOMVERTEX
{
	enum { FVF = (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1) };
	float    pos[3];
	D3DCOLOR col;
	float    uv[2];
};

class IWEditor
{

private:
	static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static IMGUI_API LRESULT ImGuiWndProcHandler(HWND, UINT msg, WPARAM wParam, LPARAM lParam);
	static void ImGuiRenderDrawLists(ImDrawData* draw_data);

public:
	static void CreateScene();
	static shared_ptr<GameObject> CreateCamera();
	static shared_ptr<GameObject> CreateGameObject();

public:
	static shared_ptr<GameObject> SelectedObject;
	static string SelectedDirectory;
	static string SelectedFile;

private:
	unique_ptr<WindowManager> _windowContainer;

	HINSTANCE				 _hInstance;
	HWND                     _hWnd;
	INT64                    _time;
	INT64                    _ticksPerSecond;
	LPDIRECT3DVERTEXBUFFER9* _ppVB;
	LPDIRECT3DINDEXBUFFER9*  _ppIB;
	LPDIRECT3DTEXTURE9       _pTexture;
	int                      _vertexBufferSize;
	int						 _indexBufferSize;

	unique_ptr<TransformSystem> _transformSystem;
	unique_ptr<MaterialSystem> _materialSystem;
	unique_ptr<RenderSystem> _renderSystem;

private:
	bool ImGuiInit();
	void ImGuiShutdown();
	bool ImGuiCreateFontsTexture();
	void ImGuiNewFrame();

public:
	IWEditor();
	~IWEditor();

	bool Init();
	void Update();
	void Release();

	LPDIRECT3DVERTEXBUFFER9* GetD3DVertexBuffer() const { return _ppVB; }
	LPDIRECT3DINDEXBUFFER9*  GetD3DIndexBuffer() const { return _ppIB; }
	int                      GetVertexBufferSize() const { return _vertexBufferSize; }
	int						 GetIndexBufferSize() const { return _indexBufferSize; }

	bool ImGuiCreateDeviceObjects();
	void ImGuiInvalidateDeviceObjects();

	const shared_ptr<WindowBase> GetWindow(const string& name) const;
};

