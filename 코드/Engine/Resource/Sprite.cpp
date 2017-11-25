#include "Sprite.h"
#include "Texture.h"

Sprite::Sprite()
	: GeometryResource(),
	_texture(nullptr)
{
}

Sprite::~Sprite()
{
}

bool Sprite::Create(void* pData, void* pResourceData, const string& name)
{
	IDirect3DDevice9* pd3dDevice = static_cast<IDirect3DDevice9*>(pData);

	SPRITEVERTEX vertex[4];
	float width = _texture->GetWidth() / 2.0f;
	float height = _texture->GetHeight() / 2.0f;

	if (FAILED(pd3dDevice->CreateVertexBuffer(sizeof(SPRITEVERTEX) * 4,
		0, SKINNEDMESHVERTEX::FVF, D3DPOOL_MANAGED, &_pVertexBuffer, NULL))) {
		return false;
	}

	if (FAILED(pd3dDevice->CreateIndexBuffer(sizeof(INDEX) * 2,
		0, D3DFMT_INDEX32, D3DPOOL_MANAGED, &_pIndexBuffer, 0)))
	{
		SafeRelease<IDirect3DVertexBuffer9>(_pVertexBuffer);
		return false;
	}

	/*float left = pSpriteInfo->left == 0 ? 0 : (float)pSpriteInfo->left / (float)_texture->GetWidth();
	float right = pSpriteInfo->right == 0 ? 0 : (float)pSpriteInfo->right / (float)_texture->GetWidth();
	float top = pSpriteInfo->top == 0 ? 1 : (float)(_texture->GetHeight() - pSpriteInfo->top) / (float)_texture->GetHeight();
	float bottom = pSpriteInfo->bottom == 1 ? 0 : (float)(_texture->GetHeight() - pSpriteInfo->bottom) / (float)_texture->GetHeight();*/

	vertex[0].position = Vector3(-width, height, 0.0f);
	vertex[1].position = Vector3(width, height, 0.0f);
	vertex[2].position = Vector3(-width, -height, 0.0f);
	vertex[3].position = Vector3(width, -height, 0.0f);
	vertex[0].texCoord = Vector2(0, 0);
	vertex[1].texCoord = Vector2(0, 0);
	vertex[2].texCoord = Vector2(0, 0);
	vertex[3].texCoord = Vector2(0, 0);

	for (int i = 0; i < 4; i++)	
		vertex[i].normal = vertex[i].position;	

	void* pVertexData;
	if (_pVertexBuffer->Lock(0, sizeof(SPRITEVERTEX) * 4, &pVertexData, 0))
	{
		SPRITEVERTEX* pVertice = static_cast<SPRITEVERTEX*>(pVertexData);

		for (int i = 0; i < 4; ++i)
		{
			*pVertice++ = vertex[i];
			_vertices->data()[i] = vertex[i].position;
		}

		_pVertexBuffer->Unlock();
	}

	void* pIndexData;
	if (_pIndexBuffer->Lock(0, sizeof(INDEX) * 2, &pIndexData, 0))
	{
		INDEX* pIndex = static_cast<INDEX*>(pIndexData);

		INDEX index;

		index._0 = 0;
		index._2 = 1;
		index._1 = 2;

		*pIndex++ = index;
		_indices->data()[0] = index;

		index._0 = 2;
		index._2 = 1;
		index._1 = 3;

		*pIndex = index;
		_indices->data()[1] = index;

		_pIndexBuffer->Unlock();
	}

	return true;
}

void Sprite::Destroy()
{
	SafeRelease<IDirect3DVertexBuffer9>(_pVertexBuffer);
	SafeRelease<IDirect3DIndexBuffer9>(_pIndexBuffer);
}