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

	D3DDISPLAYMODE d3ddm;
	if (FAILED(_pd3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		SafeRelease<IDirect3D9>(_pd3d);
		return false;
	}

	HRESULT hr;
	if (FAILED(hr = _pd3d->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		d3ddm.Format, D3DUSAGE_DEPTHSTENCIL,
		D3DRTYPE_SURFACE, D3DFMT_D16)))
	{
		if (hr == D3DERR_NOTAVAILABLE)
		{
			SafeRelease<IDirect3D9>(_pd3d);
			return false;
		}
	}
	D3DCAPS9 d3dCaps;
	if (FAILED(_pd3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dCaps)))
	{
		SafeRelease<IDirect3D9>(_pd3d);
		return false;
	}


	DWORD vp = 0;
	if (d3dCaps.VertexProcessingCaps != 0)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = GetActiveWindow();
	d3dpp.Windowed = TRUE;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
	d3dpp.FullScreen_RefreshRateInHz = 0;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

	if (FAILED(_pd3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, GetActiveWindow(), vp, &d3dpp, &_pd3dDevice)))
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
	_pd3dDevice->SetRenderState(D3DRS_STENCILENABLE, true);
	_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	return true;
}

void D3D9Core::Release()
{
	SafeRelease<ID3DXSprite>(_pd3dSprite);	
	SafeRelease<IDirect3DDevice9>(_pd3dDevice);
	SafeRelease<IDirect3D9>(_pd3d);	
}