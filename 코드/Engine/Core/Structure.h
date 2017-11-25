#pragma once

#include "Math\Math.h"
#include "BitFlag.h"

typedef struct tagRay
{
	Vector3 origin;
	Vector3 direction;
}RAY, *LPRAY;

typedef struct tagPointfloat
{
	float x;
	float y;
}POINTF, *LPPOINTF;

struct INDEX
{
#ifdef _USE_INDEX16
	WORD	_0, _1, _2;		/// WORD, 16∫Ò∆Æ ¿Œµ¶Ω∫
#else
	DWORD	_0, _1, _2;		/// WORD, 32∫Ò∆Æ ¿Œµ¶Ω∫
#endif
};

typedef struct tagVertexBufferInfo
{
	int stride;
	int verticeCount;
	DWORD usage;
	DWORD fvf;
	D3DPOOL pool;
}VERTEXBUFFERINFO, *LPVERTEXBUFFERINFO;

typedef struct tagIndexBufferInfo
{
	int size;
	DWORD usage;
	D3DPOOL pool;
}INDEXBUFFERINFO, *LPINDEXBUFFERINFO;

typedef struct tagStaticMeshVertex
{
	enum {
		FVF = (D3DFVF_XYZ |
		D3DFVF_NORMAL |
			D3DFVF_TEX3 |
			D3DFVF_TEXCOORDSIZE2(0) |
			D3DFVF_TEXCOORDSIZE3(1) |
			D3DFVF_TEXCOORDSIZE3(2))
	};

	Vector3 position;
	Vector3 normal;
	Vector2 texCoord;
	Vector3 tangent;
	Vector3 binormal;
}STATICMESHVERTEX, *LPSTATICMESHVERTEX;

typedef struct tagSkinnedMeshVertex
{
	enum {
		FVF = (D3DFVF_XYZ |
		D3DFVF_NORMAL |
			D3DFVF_TEX5 |
			D3DFVF_TEXCOORDSIZE2(0) |
			D3DFVF_TEXCOORDSIZE3(1) |
			D3DFVF_TEXCOORDSIZE3(2) |
			D3DFVF_TEXCOORDSIZE4(3) |
			D3DFVF_TEXCOORDSIZE4(4))
	};

	Vector3 position;
	Vector3 normal;
	Vector2 texCoord;
	Vector3 tangent;
	Vector3 binormal;
	float	weight[4];
	float	index[4];

}SKINNEDMESHVERTEX, *LPSKINNEDMESHVERTEX;

typedef struct tagSpriteVertex
{
	enum {
		FVF = D3DFVF_XYZ |
		D3DFVF_NORMAL |
		D3DFVF_TEX1 |
		D3DFVF_TEXCOORDSIZE2(0)
	};

	Vector3 position;
	Vector3 normal;
	Vector2 texCoord;
}SPRITEVERTEX;

typedef struct tagSkyBoxVertex
{
	enum { FVF = D3DFVF_XYZ | D3DFVF_TEX1 };

	float x, y, z, u, v;
}SKYBOXVERTEX;

struct BOXVERTEX
{
	enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };

	float x, y, z;
	DWORD color;
	BOXVERTEX() { x = 0; y = 0; z = 0; color = 0; }
	BOXVERTEX(float f1, float f2, float f3, DWORD c) { x = f1; y = f2; z = f3; color = c; }
	BOXVERTEX(const Vector3& v, DWORD c) { x = v.x; y = v.y; z = v.z; color = c; }
};