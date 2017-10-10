#include "SpriteRenderer.h"


void SpriteRenderer::Update(void* pData)
{
	IDirect3DDevice9* pd3dDevice = static_cast<IDirect3DDevice9*>(pData);
}

void SpriteRenderer::Create(void* pData, const shared_ptr<void> gameObject)
{
	_gameObject = static_pointer_cast<GameObject>(gameObject);
	_materials.reset(new MaterialVec);
}

void SpriteRenderer::Destroy()
{
	_gameObject.reset();
	_materials->clear();
	_materials.reset();
	_sprite.reset();
}

void SpriteRenderer::SetSprite(const shared_ptr<Sprite> pSprite)
{
	_sprite = pSprite;	
}
