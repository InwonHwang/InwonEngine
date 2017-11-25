#pragma once

#include "..\Core\Core.h"

class D3D9Core : public SingletonBase<D3D9Core>
{
private:
	IDirect3D9*         _pd3d;
	IDirect3DDevice9*   _pd3dDevice;
	ID3DXSprite*		_pd3dSprite;
	D3DPRESENT_PARAMETERS _d3dpp;

public:
	D3D9Core();
	~D3D9Core();

	bool Init() override;
	void Release() override;

	void SetBackBufferSize(int width, int height);
	IDirect3DDevice9* GetD3DDevice() const { return _pd3dDevice; }
	ID3DXSprite* GetD3DSprite() const { return _pd3dSprite; }
};