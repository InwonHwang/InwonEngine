#include "MaterialManager.h"
#include "..\Component\Component.h"
#include "..\Resource\Resource.h"

bool MaterialManager::_instantiated = false;

MaterialManager::MaterialManager()
{
	assert(!MaterialManager::_instantiated);
	MaterialManager::_instantiated = true;

	_pDirectionalLight.reset(new DIRECTIONALLIGHT);
	_pPointLight.reset(new POINTLIGHT);
	_pSpotLight.reset(new SPOTLIGHT);
}

MaterialManager::~MaterialManager()
{	
	MaterialManager::_instantiated = false;

	_pDirectionalLight.reset();
	_pPointLight.reset();
	_pSpotLight.reset();
}

void MaterialManager::Update()
{
	auto materials = GameObject::FindObjectsOfType<Renderer>();

	size_t materialCount = materials->size();

	shared_ptr<Material> material;

	Vector3 camPos, viewDir;

	Camera::mainCamera->GetGameObject()->GetComponent<Transform>()->GetPosition(camPos);
	Camera::mainCamera->GetGameObject()->GetComponent<Transform>()->GetAxisZ(viewDir);
	D3DXMATRIX matrix = Camera::mainCamera->GetViewProjMatrix();

	//UpdateLight();

	for (int i = 0; i < materialCount; ++i)
	{
		material = materials->data()[i]->GetComponent<Renderer>()->GetMaterial(0);

		material->GetEffect()->SetMatrix("ViewProjMatrix", &matrix);
		material->GetEffect()->SetValue("CamPos", &camPos, sizeof(Vector3));
		material->GetEffect()->SetValue("ViewDir", &viewDir, sizeof(Vector3));

		/*material->SetInt("gDLCount", _pDirectionalLight->color.size());
		material->SetValue("gDLDir", _pDirectionalLight->direction.data(), sizeof(Vector3) * _pDirectionalLight->color.size());

		material->SetInt("gPLCount", _pPointLight->color.size());
		material->SetValue("gPLPos", _pPointLight->position.data(), sizeof(Vector3) * _pPointLight->position.size());
		material->SetValue("gPLRange", _pPointLight->range.data(), sizeof(float) * _pPointLight->range.size());

		material->SetInt("gSLCount", _pSpotLight->color.size());
		material->SetValue("gSLPos", _pSpotLight->position.data(), sizeof(Vector3) * _pSpotLight->position.size());
		material->SetValue("gSLDir", _pSpotLight->direction.data(), sizeof(Vector3) * _pSpotLight->direction.size());
		material->SetValue("gSLRange", _pSpotLight->range.data(), sizeof(float) * _pSpotLight->range.size());
		material->SetValue("gSLAngle", _pSpotLight->angle.data(), sizeof(float) * _pSpotLight->angle.size());*/
	}
}

void MaterialManager::UpdateLight()
{
	_pDirectionalLight->color.swap(vector<Vector3>());
	_pDirectionalLight->direction.swap(vector<Vector3>());
	_pDirectionalLight->intensity.swap(vector<float>());
	_pPointLight->color.swap(vector<Vector3>());
	_pPointLight->position.swap(vector<Vector3>());
	_pPointLight->intensity.swap(vector<float>());
	
	auto lights = GameObject::FindObjectsOfType<Light>();

	size_t lightCount = lights->size();

	shared_ptr<Light> pLight;

	Vector3 color, direction, position;
	float intensity, range, angle;

	for (int j = 0; j < lightCount; ++j)
	{
		pLight = lights->data()[j]->GetComponent<Light>();
		Light::Type type = pLight->GetType();

		switch (type)
		{
		case Light::eDirectional:
		{
			pLight->GetColor(color);
			pLight->GetGameObject()->GetComponent<Transform>()->GetAxisZ(direction);
			intensity = pLight->GetIntensity();

			_pDirectionalLight->color.push_back(color);
			_pDirectionalLight->direction.push_back(direction);
			_pDirectionalLight->intensity.push_back(intensity);
		} break;
		case Light::ePoint:
		{
			pLight->GetColor(color);
			pLight->GetGameObject()->GetComponent<Transform>()->GetLocalPosition(position);
			intensity = pLight->GetIntensity();
			range = pLight->GetRange();

			_pPointLight->color.push_back(color);
			_pPointLight->position.push_back(position);
			_pPointLight->intensity.push_back(intensity);
			_pPointLight->range.push_back(range);
		} break;
		case Light::eSpot:
		{
			pLight->GetColor(color);
			pLight->GetGameObject()->GetComponent<Transform>()->GetPosition(position);
			pLight->GetGameObject()->GetComponent<Transform>()->GetAxisZ(direction);
			intensity = pLight->GetIntensity();
			range = pLight->GetRange();
			angle = pLight->GetAngle();

			_pSpotLight->color.push_back(color);
			_pSpotLight->position.push_back(position);
			_pSpotLight->direction.push_back(direction);
			_pSpotLight->intensity.push_back(intensity);
			_pSpotLight->range.push_back(range);
			_pSpotLight->angle.push_back(angle);
		} break;
		}
		
	}
}
