#pragma once
#include "..\Core\Core.h"
#include "..\Resource\Resource.h"

class Light :  public ComponentBase//, public std::enable_shared_from_this<Light>
{
public:
	enum Type
	{
		eDirectional,
		ePoint,
		eSpot
	};

public:
	Light() {}
	virtual ~Light() {}

	virtual void Update(void* pData) override;
	virtual void Create(void* pData, const shared_ptr<void> gameObject) override;
	virtual void Destroy() override;

	void SetType(Light::Type type);
	void SetIntensity(float intensity);
	void SetRange(float range);
	void SetAngle(float angle);
	void SetColor(const Vector3& color);

	Light::Type GetType() const;
	float GetIntensity() const;
	float GetRange() const;
	float GetAngle() const;
	void GetColor(Vector3& color) const;

private:
	shared_ptr<LIGHTINFO> _lightInfo;
};

