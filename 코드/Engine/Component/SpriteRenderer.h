#pragma once
#include "Renderer.h"

class SpriteRenderer final : public Renderer//, public std::enable_shared_from_this<SpriteRenderer>
{
private:
	shared_ptr<Sprite> _sprite;

public:
	SpriteRenderer(const shared_ptr<GameObject> gameObject);
	~SpriteRenderer();

	virtual void Update(void* pData) override;
	virtual void Create(void* pData) override;
	virtual void Destroy() override;

	virtual shared_ptr<void> Clone() override { return nullptr; }
	virtual void FromJson(const Json::Value& json) override {}
	virtual void ToJson(Json::Value& json) override {}

	void SetSprite(const shared_ptr<Sprite> sprite) { _sprite = sprite; }
	const shared_ptr<Sprite> GetSprite() const { return _sprite; }
};

