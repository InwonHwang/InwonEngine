#include "TransformSystem.h"
#include "..\Graphics\Graphics.h"
#include "..\Component\Component.h"
#include "..\Resource\Resource.h"


TransformSystem::TransformSystem(shared_ptr<D3D9Core>& d3dCore) : _d3dCore(d3dCore)
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

		if (transform->GetComponent<SkinnedMeshRenderer>())	{
			transform->SetLocalRotation(Quaternion::Euler(0.0f, 0.0f, 0.0f));
			transform->SetLocalPosition(Vector3(0.0f, 0.0f, 0.0f));
		}

		D3DXMATRIX sm, rm, tm;

		auto scale = transform->GetLocalScale();
		auto rotation = transform->GetLocalRotation();
		auto position = transform->GetLocalPosition();

		D3DXMatrixScaling(&sm, scale->x, scale->y, scale->z);
		D3DXMatrixRotationQuaternion(&rm, rotation.get());
		D3DXMatrixTranslation(&tm, position->x, position->y, position->z);

		D3DXMATRIX resultMatrix = sm * rm * tm;

		transform->SetWorldMatrix(resultMatrix);
	}
}

void TransformSystem::Update()
{
	if (!Scenes->GetCurrentScene()) return;

	processSRT();

	auto transformObjects = GameObject::FindObjectsOfType<Transform>();

	for (auto transformObject : *transformObjects) {

		auto transform = transformObject->GetComponent<Transform>();

		D3DXMATRIX m;
		D3DXMatrixIdentity(&m);

		auto parentMatrix = transform->GetParent() ? *transform->GetParent()->GetWorldMatrix() : m;
		
		auto matWorld = *transform->GetWorldMatrix() * parentMatrix;

		transform->SetWorldMatrix(matWorld);
	}
}

