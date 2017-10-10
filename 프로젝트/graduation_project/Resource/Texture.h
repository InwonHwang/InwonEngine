#pragma once
#include "..\Core\Core.h"

class Texture : public ResourceBase
{
public:
	enum State
	{
		eMipmap
	};
public:
	Texture() : _pTexture(nullptr) {}
	~Texture();

	virtual bool Create(void* pData, void* pResourceData, const string& name) override;
	virtual void Destroy() override;

	int GetWidth() const;
	int GetHeight() const;	
	IDirect3DTexture9* GetD3DTexture() const;

private:	
	IDirect3DTexture9*		_pTexture;
	int						_width;
	int						_height;
};

inline int Texture::GetWidth() const
{
	return _width;
}

inline int Texture::GetHeight() const
{
	return _height;
}

inline IDirect3DTexture9* Texture::GetD3DTexture() const
{
	return _pTexture;
}
