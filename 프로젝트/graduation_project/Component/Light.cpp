#include "Light.h"

void Light::Update(void* pData)
{

}

void Light::Create(void* pData, const shared_ptr<void> gameObject)
{
	_gameObject = static_pointer_cast<GameObject>(gameObject);

	_lightInfo.reset(new LIGHTINFO);

	_lightInfo->type.Set(eDirectional);
	_lightInfo->color = Vector3(1.0f, 1.0f, 1.0f);
	_lightInfo->intensity = 1;
}

void Light::Destroy()
{
	//ComponentPool<Light>* pCameraPool = static_cast<ComponentPool<Light>*>(_pComponentPool);
	//pCameraPool->Destroy(shared_from_this());

	_gameObject.reset();
	_lightInfo.reset();
}

void Light::SetType(Light::Type type)
{
	switch (type)
	{
	case Light::Type::eDirectional:
	{
		_lightInfo.reset(new LIGHTINFO);
		_lightInfo->type.Set(eDirectional);
		_lightInfo->color = Vector3(1.0f, 1.0f, 1.0f);
		_lightInfo->intensity = 1;
	} break;

	case Light::Type::eSpot:
	{
		_lightInfo.reset(new SPOTLIGHTINFO);
		_lightInfo->type.Set(eSpot);
		_lightInfo->color = Vector3(1.0f, 1.0f, 1.0f);
		_lightInfo->intensity = 1;
		std::static_pointer_cast<SPOTLIGHTINFO>(_lightInfo)->range = 10;
		std::static_pointer_cast<SPOTLIGHTINFO>(_lightInfo)->angle = 30;
		
	} break;

	case Light::Type::ePoint:
	{
		_lightInfo.reset(new POINTLIGHTINFO);
		_lightInfo->type.Set(ePoint);
		_lightInfo->color = Vector3(1.0f, 1.0f, 1.0f);
		_lightInfo->intensity = 1;
		std::static_pointer_cast<POINTLIGHTINFO>(_lightInfo)->range = 10;
	} break;

	}
	_lightInfo->type.Set(type);
}

void Light::SetIntensity(float intensity)
{
	_lightInfo->intensity = intensity;
}

void Light::SetRange(float range)
{
	if (_lightInfo->type.Test(Light::eDirectional)) return;

	std::static_pointer_cast<POINTLIGHTINFO>(_lightInfo)->range = range;
}

void Light::SetAngle(float angle)
{
	if (!_lightInfo->type.Test(Light::eSpot)) return;

	std::static_pointer_cast<SPOTLIGHTINFO>(_lightInfo)->angle = angle;
}

void Light::SetColor(const Vector3& color)
{
	_lightInfo->color = color;
}

Light::Type Light::GetType() const
{
	if (_lightInfo->type.Test(eDirectional))
		return eDirectional;
	else if (_lightInfo->type.Test(ePoint))
		return ePoint;
	else
		return eSpot;
}

float Light::GetIntensity() const
{
	return std::static_pointer_cast<DIRECTIONALLIGHTINFO>(_lightInfo)->intensity;
}

float Light::GetRange() const
{
	if (_lightInfo->type.Test(Light::eDirectional)) return - 1;

	return std::static_pointer_cast<POINTLIGHTINFO>(_lightInfo)->range;
}

float Light::GetAngle() const
{
	if (!_lightInfo->type.Test(Light::eSpot)) return -1;

	return std::static_pointer_cast<SPOTLIGHTINFO>(_lightInfo)->angle;
}

void Light::GetColor(Vector3& color) const
{
	color = _lightInfo->color;
}