#pragma once

#include "..\Core\Core.h"

class MaterialManager
{
	typedef struct tagLight
	{
		vector<Vector3> color;
		vector<float> intensity;
	}LIGHT;

	typedef struct tagDirectionalLight : public tagLight
	{
		vector<Vector3> direction;
	}DIRECTIONALLIGHT;

	typedef struct tagPointLight : public tagLight
	{
		vector<Vector3> position;
		vector<float> range;
	}POINTLIGHT;

	typedef struct tagSpotLight : public tagPointLight
	{
		vector<Vector3> direction;
		vector<float> angle;
	}SPOTLIGHT;

	static bool _instantiated;

public:
	MaterialManager::MaterialManager();
	MaterialManager::~MaterialManager();

	void Update();

private:
	void UpdateLight();

private:
	shared_ptr<DIRECTIONALLIGHT>		_pDirectionalLight;
	shared_ptr<POINTLIGHT>				_pPointLight;
	shared_ptr<SPOTLIGHT>				_pSpotLight;
};

