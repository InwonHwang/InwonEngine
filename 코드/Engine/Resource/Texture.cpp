#include "Texture.h"

Texture::Texture()
	: ResourceBase(),
	_pTexture(nullptr)
{
}

Texture::~Texture()
{
}

bool Texture::Create(void* pData, void* pResourceData, const string& name)
{
	*_name = name;

	IDirect3DDevice9* pd3dDevice = static_cast<IDirect3DDevice9*>(pData);
	string* fileName = static_cast<string*>(pResourceData);
	HRESULT hr = 0;

	if (FAILED(hr = D3DXCreateTextureFromFile(pd3dDevice, fileName->c_str(), &_pTexture)))
	{
		return false;
	}

	D3DSURFACE_DESC desc;
	_pTexture->GetLevelDesc(0, &desc);
	_width = desc.Width;
	_height = desc.Height;

	return true;
}

void Texture::Destroy()
{
	SafeRelease<IDirect3DTexture9>(_pTexture);
}