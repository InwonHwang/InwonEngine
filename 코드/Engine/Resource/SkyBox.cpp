#include "SkyBox.h"
#include "..\Singleton\Singleton.h"
#include "Effect.h"
#include "Texture.h"

SkyBox::SkyBox()
	: GeometryResource(),
	_textures(nullptr)
{
	_textures = make_unique<TextureMap>();
}

SkyBox::~SkyBox()
{
}

bool SkyBox::Create(void* pData, void* pResourceData, const string& name)
{
	string* pPath = static_cast<string*>(pResourceData);
	IDirect3DDevice9* pd3dDevice = static_cast<IDirect3DDevice9*>(pData);

	Json::Value json;
	JsonUtility::ReadFromFile(*pPath, json);

	*_name = name;

	for (auto key : json["textures"].getMemberNames()) {
		auto texture = Resources->Create<Texture>(json["textures"][key].asString());
		Direction d;

		if (key.compare("top") == 0) SetTexture(eTop, texture);
		if (key.compare("front") == 0) SetTexture(eFront, texture);
		if (key.compare("back") == 0) SetTexture(eBack, texture);
		if (key.compare("left") == 0) SetTexture(eLeft, texture);
		if (key.compare("right") == 0) SetTexture(eRight, texture);
		if (key.compare("bottom") == 0) SetTexture(eBottom, texture);
	}

	float size = 3000;
	SKYBOXVERTEX vertices[24] =
	{
		{ -size, size, -size, 0.0f, 0.0f },		// Top vertices
		{ -size, size,  size, 0.0f, 1.0f },
		{ size, size, -size, 1.0f, 0.0f },
		{ size, size,  size, 1.0f, 1.0f },

		{ -size,  size, size, 0.0f, 0.0f },		// Front vertices
		{ -size, -size, size, 0.0f, 1.0f },
		{ size,  size, size, 1.0f, 0.0f },
		{ size, -size, size, 1.0f, 1.0f },

		{ size,  size, -size, 0.0f, 0.0f },		// Back vertices
		{ size, -size, -size, 0.0f, 1.0f },
		{ -size,  size, -size, 1.0f, 0.0f },
		{ -size, -size, -size, 1.0f, 1.0f },

		{ -size,  size, -size, 0.0f, 0.0f },		// Left vertices
		{ -size, -size, -size, 0.0f, 1.0f },
		{ -size,  size,  size, 1.0f, 0.0f },
		{ -size, -size,  size, 1.0f, 1.0f },

		{ size,  size,  size, 0.0f, 0.0f },		// Right vertices
		{ size, -size,  size, 0.0f, 1.0f },
		{ size,  size, -size, 1.0f, 0.0f },
		{ size, -size, -size, 1.0f, 1.0f },

		{ -size, -size,  size, 0.0f, 0.0f },		// Bottom vertices
		{ -size, -size, -size, 0.0f, 1.0f },
		{ size, -size,  size, 1.0f, 0.0f },
		{ size, -size, -size, 1.0f, 1.0f }
	};

	INDEX indices[12] =
	{
		{ 0,1,2 },{ 1,3,2 },
		{ 4,5,6 },{ 5,7,6 },
		{ 8,9,10 },{ 9,11,10 },
		{ 12,13,14 },{ 13,15,14 },
		{ 16,17,18 },{ 18,19,17 },
		{ 20,21,22 },{ 22,23,21 }
	};

	if (FAILED(pd3dDevice->CreateVertexBuffer(sizeof(SKYBOXVERTEX) * 24,
		0, SKYBOXVERTEX::FVF, D3DPOOL_MANAGED, &_pVertexBuffer, NULL)))
		return false;

	void * pBuffer = nullptr;

	if (SUCCEEDED(_pVertexBuffer->Lock(0, 24 * sizeof(SKYBOXVERTEX), &pBuffer, 0)))
	{
		memcpy(pBuffer, vertices, sizeof(vertices));
		_pVertexBuffer->Unlock();
	}

	return true;
}

void SkyBox::Destroy()
{
	SafeRelease<IDirect3DVertexBuffer9>(_pVertexBuffer);
	SafeRelease<IDirect3DIndexBuffer9>(_pIndexBuffer);
}