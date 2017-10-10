#include "Texture.h"

Texture::~Texture()
{
}

bool Texture::Create(void* pData, void* pResourceData, const string& name)
{
	_name = make_shared<string>(name);
	//IDirect3DDevice9* pd3dDevice = static_cast<IDirect3DDevice9*>(pData);
	//string* fileName = static_cast<string*>(pResourceData);
	//HRESULT hr = 0;

	//if (FAILED(hr = D3DXCreateTextureFromFile(pd3dDevice, fileName->c_str(), &_pTexture)))
	///*if (FAILED(hr = D3DXCreateTextureFromFileEx(pd3dDevice, fileName->c_str(),
	//	D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, NULL, D3DFMT_UNKNOWN,
	//	D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0xff000000,
	//	NULL, NULL, &_pTexture)))*/
	//{
	//	return;
	//}

	//D3DSURFACE_DESC desc;
	//_pTexture->GetLevelDesc(0, &desc);
	//_width = desc.Width;
	//_height = desc.Height;

	return true;
}

void Texture::Destroy()
{
	SafeRelease<IDirect3DTexture9>(_pTexture);
}