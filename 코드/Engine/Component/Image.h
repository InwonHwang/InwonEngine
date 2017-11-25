#pragma once

#include "UI.h"

class Image final : public UI
{
private:
	shared_ptr<Texture>	_texture;

public:
	Image(const shared_ptr<GameObject> gameObject);
	~Image();
		
	virtual void Update(void* pData) override;
	virtual void Create(void* pData) override;
	virtual void Destroy() override;

	virtual shared_ptr<void> Clone() override { return nullptr; }
	virtual void FromJson(const Json::Value& json) override {}
	virtual void ToJson(Json::Value& json) override {}

	virtual void Draw(ID3DXSprite*	pSprite) override;

	void SetTexture(const shared_ptr<Texture> texture) { _texture = texture; }
	const shared_ptr<Texture> GetTexture() const { return _texture; }
};