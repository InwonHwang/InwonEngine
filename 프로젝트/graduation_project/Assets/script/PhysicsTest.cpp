#include "PhysicsTest.h"

void PhysicsTest::Awake()
{

}

void PhysicsTest::Update()
{
	if (Input->GetKeyDown(InputManager::KeyCode::eKey_Num0) && _pTempObject) {
		_pTempObject->GetComponent<Transform>()->SetLocalPosition(Vector3(0, 50, 0));
	}

	if (Input->GetMouseButtonDown(InputManager::MouseCode::eButton_Left))
	{
		Vector2 mousePoint = Input->GetMousePoint();
		
		RAY ray = Camera::mainCamera->ScreenPointToRay(mousePoint);
		shared_ptr<GameObject> hitObj;
		Vector3 hitPoint;

		//if (Physics::Raycast(ray, 10000.f, hitPoint, hitObj))
		//{
			//SceneMgr->GetCurScene()->Find("player1")->GetComponent<Transform>()->SetLocalPosition(hitPoint);
		//}
	}
}

void PhysicsTest::OnTrigger(shared_ptr<Collider> pCollider)
{
	shared_ptr<Animator> animator = pCollider->GetGameObject()->GetComponent<Animator>();

	if (animator)
		animator->CrossFade(2);
	_pTempObject = pCollider->GetGameObject();
}