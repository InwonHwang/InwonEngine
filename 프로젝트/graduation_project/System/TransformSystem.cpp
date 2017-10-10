#include "TransformSystem.h"
#include "..\Graphics\Graphics.h"
#include "..\Component\Component.h"
#include "..\Resource\Resource.h"


TransformSystem::TransformSystem(shared_ptr<D3D9Core>& device) : _device(device)
{
}

TransformSystem::~TransformSystem()
{
}

void TransformSystem::processSRT()
{
	auto transformObjects = GameObject::FindObjectsOfType<Transform>();

	for (auto transformObject : *transformObjects) {

		auto transform = transformObject->GetComponent<Transform>();

		D3DXMATRIX scale, rotation, translation;
		Vector3 localScale, localPosition;
		Quaternion localRotation;

		transform->GetLocalScale(localScale);
		transform->GetLocalRotation(localRotation);
		transform->GetLocalPosition(localPosition);

		D3DXMatrixScaling(&scale, localScale.x, localScale.y, localScale.z);
		D3DXMatrixRotationQuaternion(&rotation, &localRotation);
		D3DXMatrixTranslation(&translation, localPosition.x, localPosition.y, localPosition.z);

		D3DXMATRIX resultMatrix = scale * rotation * translation;

		transform->SetLocalMatrix(resultMatrix);
	}
}

void TransformSystem::Update()
{
	processSRT();

	auto transformObjects = GameObject::FindObjectsOfType<Transform>();

	for (auto transformObject : *transformObjects) {

		auto transform = transformObject->GetComponent<Transform>();
		auto parent = transform->GetParent();

		auto matWorld = parent ? *transform->GetLocalMatrix() * *parent->GetWorldMatrix() : *transform->GetLocalMatrix();

		transform->SetWorldMatrix(matWorld);

		_device->GetD3DDevice()->SetTransform(D3DTS_WORLD, &matWorld);
	}
}

