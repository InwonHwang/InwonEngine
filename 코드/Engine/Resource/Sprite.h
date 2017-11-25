#pragma once

#include "GeometryResource.h"

class Texture;

class Sprite : public GeometryResource
{
private:
	shared_ptr<Texture> _texture;

public:
	Sprite();
	~Sprite();

	virtual bool Create(void* pData, void* pResourceData, const string& name) override;
	virtual void Destroy() override;
	virtual int GetStride() const override { return sizeof(SPRITEVERTEX); }
	virtual unsigned long GetFVF() const override { return SPRITEVERTEX::FVF; }

	const shared_ptr<Texture> GetD3DTexture() const { return _texture; }
};
