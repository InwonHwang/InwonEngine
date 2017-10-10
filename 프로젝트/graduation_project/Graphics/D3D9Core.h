#pragma once

#include "..\Core\Core.h"

class D3D9Core : public SingletonBase<D3D9Core>
{
public:
	D3D9Core();
	~D3D9Core();

	bool Init() override;
	void Release() override;

	IDirect3DDevice9* GetD3DDevice() const;
	ID3DXSprite* GetD3DSprite() const;

private:	
	IDirect3D9*         _pd3d;
	IDirect3DDevice9*   _pd3dDevice;
	ID3DXSprite*		_pd3dSprite;
};

inline IDirect3DDevice9* D3D9Core::GetD3DDevice() const
{
	return _pd3dDevice;
}

inline ID3DXSprite* D3D9Core::GetD3DSprite() const
{
	return _pd3dSprite;
}