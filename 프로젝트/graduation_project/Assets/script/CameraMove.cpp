#include "CameraMove.h"

void CameraMove::Awake()
{

}

void CameraMove::Update()
{
	// Camera ¿Ãµø
	shared_ptr<GameObject> mainCamera = Camera::mainCamera->GetGameObject();

	Vector3 camPos;
	mainCamera->GetComponent<Transform>()->GetLocalPosition(camPos);
	if (Input->GetKeyDown(InputManager::KeyCode::eKey_W)) mainCamera->GetComponent<Transform>()->SetLocalPosition(camPos + Vector3(0, 0, 1));
	if (Input->GetKeyDown(InputManager::KeyCode::eKey_S)) mainCamera->GetComponent<Transform>()->SetLocalPosition(camPos + Vector3(0, 0, -1));
	if (Input->GetKeyDown(InputManager::KeyCode::eKey_A)) mainCamera->GetComponent<Transform>()->SetLocalPosition(camPos + Vector3(-1, 0, 0));
	if (Input->GetKeyDown(InputManager::KeyCode::eKey_D)) mainCamera->GetComponent<Transform>()->SetLocalPosition(camPos + Vector3(1, 0, 0));
	if (Input->GetKeyDown(InputManager::KeyCode::eKey_R)) mainCamera->GetComponent<Transform>()->SetLocalPosition(camPos + Vector3(0, 1, 0));
	if (Input->GetKeyDown(InputManager::KeyCode::eKey_F)) mainCamera->GetComponent<Transform>()->SetLocalPosition(camPos + Vector3(0, -1, 0));

	Vector3 eulerAngle;
	Vector3 resultAngle;
	mainCamera->GetComponent<Transform>()->GetLocalEulerAngle(eulerAngle);
	if (Input->GetKeyDown(InputManager::KeyCode::eKey_I))
	{
		resultAngle = eulerAngle + Vector3(-2.0f, 0, 0);
		mainCamera->GetComponent<Transform>()->SetLocalRotation(Quaternion::Euler(resultAngle.x, resultAngle.y, resultAngle.z));
	}
	if (Input->GetKeyDown(InputManager::KeyCode::eKey_K))
	{
		resultAngle = eulerAngle + Vector3(2.0f, 0, 0);
		mainCamera->GetComponent<Transform>()->SetLocalRotation(Quaternion::Euler(resultAngle.x, resultAngle.y, resultAngle.z));
	}
	if (Input->GetKeyDown(InputManager::KeyCode::eKey_J))
	{
		resultAngle = eulerAngle + Vector3(0, -2.0f, 0);
		mainCamera->GetComponent<Transform>()->SetLocalRotation(Quaternion::Euler(resultAngle.x, resultAngle.y, resultAngle.z));
	}
	if (Input->GetKeyDown(InputManager::KeyCode::eKey_L))
	{
		resultAngle = eulerAngle + Vector3(0, 2.0f, 0);
		mainCamera->GetComponent<Transform>()->SetLocalRotation(Quaternion::Euler(resultAngle.x, resultAngle.y, resultAngle.z));
	}
}

void CameraMove::OnTrigger(shared_ptr<Collider> pCollider)
{
}