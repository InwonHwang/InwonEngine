#include "MaterialSystem.h"
#include "..\Component\Component.h"
#include "..\Resource\Resource.h"

MaterialSystem::MaterialSystem(const shared_ptr<D3D9Core> d3dCore)
	: _d3dCore(d3dCore)
{
}

MaterialSystem::~MaterialSystem()
{	
}

void MaterialSystem::updateCamera()
{
	auto pd3dDevice = _d3dCore->GetD3DDevice();
	auto cameraObjects = GameObject::FindObjectsOfType<Camera>();

	for (auto cameraObject : *cameraObjects)
	{
		auto camera = cameraObject->GetComponent<Camera>();
		auto transform = cameraObject->GetComponent<Transform>();

		auto viewMatrix = camera->GetViewMatrix();
		auto projMatrix = camera->GetProjMatrix();

		camera->GetFrustum()->Make((*viewMatrix) * (*projMatrix));

		auto eye = transform->GetLocalPosition();
		auto up = transform->GetAxisY();
		auto lookAt = *eye + *transform->GetAxisZ();

		D3DXMatrixLookAtRH(viewMatrix.get(), eye.get(), &lookAt, up.get());
		D3DXMatrixPerspectiveFovRH(projMatrix.get(), D3DXToRadian(45), 1.0f, 0.01f, 5000.0f);

		pd3dDevice->SetTransform(D3DTS_VIEW, viewMatrix.get());
		pd3dDevice->SetTransform(D3DTS_PROJECTION, projMatrix.get());
	}
}

void MaterialSystem::updateLight()
{

}

void MaterialSystem::Update()
{
	if (!Scenes->GetCurrentScene()) return;

	updateCamera();

	auto rendererObjects = GameObject::FindObjectsOfType<Renderer>();

	auto viewProjMatrix = *Camera::mainCamera->GetViewMatrix() * *Camera::mainCamera->GetProjMatrix();

	auto camPos = Camera::mainCamera->GetComponent<Transform>()->GetPosition();
	auto viewDir = Camera::mainCamera->GetComponent<Transform>()->GetAxisZ();

	for (auto rendererObject : *rendererObjects)
	{
		auto renderer = rendererObject->GetComponent<Renderer>();

		for (int i = 0; i < renderer->GetMaterialCount(); ++i)
		{
			auto effect = renderer->GetMaterial(i)->GetEffect()->GetD3DEffect();

			effect->SetMatrix("ViewProjMatrix", &viewProjMatrix);
			//effect->SetValue("CamPos", &camPos, sizeof(Vector3));
			//effect->SetValue("ViewDir", &viewDir, sizeof(Vector3));
		}
	}
}
