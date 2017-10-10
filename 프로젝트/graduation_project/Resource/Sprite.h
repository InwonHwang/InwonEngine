#pragma once

#include "GeometryResource.h"

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

class Sprite : public GeometryResource
{
private:
	shared_ptr<Texture> _texture;

public:
	Sprite() : GeometryResource(), _texture(nullptr) {}		
	~Sprite() {}

	virtual bool Create(void* pData, void* pResourceData, const string& name) override;
	virtual void Destroy() override;
	virtual int GetStride() const override { return sizeof(SPRITEVERTEX); }
	virtual unsigned long GetFVF() const override { return SPRITEVERTEX::FVF; }

	// friend class SpriteRenderer private????? ????
	shared_ptr<Texture>	GetTexture() const;


};

inline shared_ptr<Texture>	Sprite::GetTexture() const
{
	return _texture;
}
