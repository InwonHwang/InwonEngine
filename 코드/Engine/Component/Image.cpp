#include "Image.h"

Image::Image(const shared_ptr<GameObject> gameObject)
	: UI(gameObject),
	_texture(nullptr)
{
}

Image::~Image()
{
}

void Image::Update(void* pData)
{
	ID3DXSprite* pd3dSprite = static_cast<ID3DXSprite *>(pData);
	Draw(pd3dSprite);
}

void Image::Create(void* pData)
{
}

void Image::Destroy()
{
}

void Image::Draw(ID3DXSprite* pSprite)
{
	if (!_texture) return;

	pSprite->Begin(D3DXSPRITE_ALPHABLEND);    // // begin sprite drawing with transparency

	IDirect3DTexture9* texture = _texture->GetD3DTexture();
						 
	D3DXVECTOR3 center(0.0f, 0.0f, 0.0f);   
	D3DXVECTOR3 position(50.0f, 50.0f, 0.0f);
	
	pSprite->Draw(_texture->GetD3DTexture(), NULL, &center, &position, D3DCOLOR_ARGB(255, 255, 255, 255));

	pSprite->End();    // end sprite drawing
}