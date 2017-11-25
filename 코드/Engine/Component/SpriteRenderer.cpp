#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer(const shared_ptr<GameObject> gameObject)
	: Renderer(gameObject),
	_sprite(nullptr)
{
}

SpriteRenderer::~SpriteRenderer()
{
}

void SpriteRenderer::Update(void* pData)
{
	IDirect3DDevice9* pd3dDevice = static_cast<IDirect3DDevice9*>(pData);
}

void SpriteRenderer::Create(void* pData)
{
}

void SpriteRenderer::Destroy()
{
	Renderer::Destroy();
}