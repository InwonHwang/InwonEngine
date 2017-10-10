#pragma once
#include "Renderer.h"

class SpriteRenderer final : public Renderer//, public std::enable_shared_from_this<SpriteRenderer>
{
public:
	SpriteRenderer()
		:
		Renderer(),
		_sprite(nullptr)
	{
	}
	
	~SpriteRenderer() {}

	virtual void Update(void* pData) override;
	virtual void Create(void* pData, const shared_ptr<void> gameObject) override;
	virtual void Destroy() override;

	void SetSprite(const shared_ptr<Sprite> pSprite);	

private:
	shared_ptr<Sprite>			_sprite;	
};

