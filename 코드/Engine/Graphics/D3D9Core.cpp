#include "D3D9Core.h"
#include "Memory.h"

D3D9Core::D3D9Core()
	:
	_pd3d(nullptr),
	_pd3dDevice(nullptr),
	_pd3dSprite(nullptr)
{
}

D3D9Core::~D3D9Core()
{
}

bool D3D9Core::Init()
{
	if (NULL == (_pd3d = Direct3DCreate9(D3D_SDK_VERSION)))
		return false;

	ZeroMemory(&_d3dpp, sizeof(_d3dpp));

	//_d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	//_d3dpp.BackBufferCount = 1;
	//_d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	//_d3dpp.MultiSampleQuality = 0;
	//_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	//_d3dpp.hDeviceWindow = GetActiveWindow();
	//_d3dpp.Windowed = TRUE;
	//_d3dpp.EnableAutoDepthStencil = TRUE;
	//_d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	//_d3dpp.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
	//_d3dpp.FullScreen_RefreshRateInHz = 0;
	//_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

	_d3dpp.Windowed = TRUE;
	_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	_d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	_d3dpp.EnableAutoDepthStencil = TRUE;
	_d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;


	if (FAILED(_pd3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, GetActiveWindow(), D3DCREATE_HARDWARE_VERTEXPROCESSING, &_d3dpp, &_pd3dDevice)))
	{
		SafeRelease<IDirect3D9>(_pd3d);
		return false;
	}

	if (FAILED(D3DXCreateSprite(_pd3dDevice, &_pd3dSprite)))
	{
		SafeRelease<IDirect3DDevice9>(_pd3dDevice);
		SafeRelease<IDirect3D9>(_pd3d);
		return false;
	}	

	_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	_pd3dDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
	_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	_pd3dDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	return true;
}

void D3D9Core::Release()
{
	SafeRelease<ID3DXSprite>(_pd3dSprite);	
	SafeRelease<IDirect3DDevice9>(_pd3dDevice);
	SafeRelease<IDirect3D9>(_pd3d);	
}

void D3D9Core::SetBackBufferSize(int width, int height)
{
	_d3dpp.BackBufferWidth = width;
	_d3dpp.BackBufferHeight = height;
	_pd3dDevice->Reset(&_d3dpp);
}