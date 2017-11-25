#pragma once

#include "GeometryResource.h"

class Texture;
class Effect;

class SkyBox : public GeometryResource
{
	typedef map<int, shared_ptr<Texture>> TextureMap;
public:
	enum Direction : int { eTop = 0, eFront, eBack, eRight,eLeft, eBottom };

private:
	unique_ptr<TextureMap> _textures;

public:
	SkyBox();
	~SkyBox();

	virtual bool Create(void* pData, void* pResourceData, const string& name) override;
	virtual void Destroy() override;
	virtual int GetStride() const override { return sizeof(SKYBOXVERTEX); }
	virtual unsigned long GetFVF() const override { return SKYBOXVERTEX::FVF; }

	void SetTexture(int direction, const shared_ptr<Texture> texture) { _textures->insert(make_pair(direction, texture)); }
	const shared_ptr<Texture> GetTexture(int direction) const { return (*_textures)[direction]; }
};
