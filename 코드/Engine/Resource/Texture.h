#pragma once
#include "..\Core\Core.h"

class Texture : public ResourceBase
{
private:
	IDirect3DTexture9*		_pTexture;
	int						_width;
	int						_height;

public:
	Texture();
	~Texture();

	virtual bool Create(void* pData, void* pResourceData, const string& name) override;
	virtual void Destroy() override;

	int GetWidth() const { return _width; }
	int GetHeight() const { return _height; }
	IDirect3DTexture9* GetD3DTexture() const { return _pTexture; }
};
