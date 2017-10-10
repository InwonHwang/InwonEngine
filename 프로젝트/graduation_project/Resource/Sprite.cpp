#include "Sprite.h"

bool Sprite::Create(void* pData, void* pResourceData, const string& name)
{
	_name = make_shared<string>(name);

	//IDirect3DDevice9* pd3dDevice = static_cast<IDirect3DDevice9*>(pData);
	//SPRITEINFO* pSpriteInfo = static_cast<SPRITEINFO*>(pResourceData);
	//_texture = pSpriteInfo->texture;
	//
	//VERTEXBUFFERINFO vi;
	//vi.stride = sizeof(SPRITEVERTEX);
	//vi.verticeCount = 4;
	//vi.fvf = SPRITEVERTEX::FVF;
	//vi.usage = 0;
	//vi.pool = D3DPOOL_MANAGED;

	//_vertexBuffer = ResourceMgr->Create<VertexBuffer>(_name + "_vb", &vi);

	//INDEXBUFFERINFO ii;
	//ii.size = sizeof(INDEX) * 2;
	//ii.usage = 0;
	//ii.pool = D3DPOOL_MANAGED;

	//_indexBuffer = ResourceMgr->Create<IndexBuffer>(_name + "_ib", &ii);

	//_vertices.reset(new vector<Vector3>());
	//_vertices->resize(4);

	//_indices.reset(new vector<INDEX>());
	//_indices->resize(2);

	//// VertexBuffer Lock
	//void* pVertexData;
	//SPRITEVERTEX vertex[4];
	//float width = _texture->GetWidth() / 2.0f;
	//float height = _texture->GetHeight() / 2.0f;

	//float left = pSpriteInfo->left == 0 ? 0 : (float)pSpriteInfo->left / (float)_texture->GetWidth();
	//float right = pSpriteInfo->right == 0 ? 0 : (float)pSpriteInfo->right / (float)_texture->GetWidth();
	//float top = pSpriteInfo->top == 0 ? 1 : (float)(_texture->GetHeight() - pSpriteInfo->top) / (float)_texture->GetHeight();
	//float bottom = pSpriteInfo->bottom == 1 ? 0 : (float)(_texture->GetHeight() - pSpriteInfo->bottom) / (float)_texture->GetHeight();

	//vertex[0].position = Vector3(-width, height, 0.0f);
	//vertex[1].position = Vector3(width, height, 0.0f);
	//vertex[2].position = Vector3(-width, -height, 0.0f);
	//vertex[3].position = Vector3(width, -height, 0.0f);
	//vertex[0].texCoord = Vector2(left, top);
	//vertex[1].texCoord = Vector2(right, top);
	//vertex[2].texCoord = Vector2(left, bottom);
	//vertex[3].texCoord = Vector2(right, bottom);

	//for (int i = 0; i < 4; i++)
	//{
	//	vertex[i].normal = vertex[i].position;
	//}

	//if (_vertexBuffer->Lock(0, sizeof(SPRITEVERTEX) * 4, &pVertexData, 0))
	//{
	//	SPRITEVERTEX* pVertice = static_cast<SPRITEVERTEX*>(pVertexData);

	//	for (int i = 0; i < 4; ++i)
	//	{
	//		*pVertice++ = vertex[i];
	//		_vertices->data()[i] = vertex[i].position;
	//	}

	//	_vertexBuffer->Unlock();
	//}

	//// IndexBuffer Lock
	//void* pIndexData;

	//if (_indexBuffer->Lock(0, sizeof(INDEX) * 2, &pIndexData, 0))
	//{
	//	INDEX* pIndex = static_cast<INDEX*>(pIndexData);

	//	INDEX index;

	//	index._0 = 0;
	//	index._1 = 1;
	//	index._2 = 2;

	//	*pIndex++ = index;
	//	_indices->data()[0] = index;

	//	index._0 = 2;
	//	index._1 = 1;
	//	index._2 = 3;

	//	*pIndex = index;
	//	_indices->data()[1] = index;

	//	_indexBuffer->Unlock();
	//}

	return true;
}

void Sprite::Destroy()
{
	_vertices.reset();
	_indices.reset();
	_texture.reset();
}