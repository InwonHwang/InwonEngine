#pragma once

#include "UI.h"

class Image final : public UI
{
public:
	Image() : UI(), _texture(nullptr) {}
	~Image() {}
		
	virtual void Update(void* pData) override;
	virtual void Create(void* pData, const shared_ptr<void> gameObject) override;
	virtual void Destroy() override;

	virtual void Draw(ID3DXSprite*	pSprite) override;

	void SetTexture(const shared_ptr<Texture> pTexture);

private:
	shared_ptr<Texture>		_texture;
};