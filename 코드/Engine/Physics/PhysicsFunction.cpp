#pragma once

#include "Physics.h"

bool Raycast(const RAY& ray, float distance, Vector3& hitPoint, shared_ptr<GameObject>& gameObject)
{
	//shared_ptr<ComponentPool<RigidBody>> pPool = ComponentMgr->GetComponentPool<RigidBody>();

	//static float up = 0.f;
	//static float dir = 1.f;

	//m_dynamicsWorld->updateAabbs();
	//m_dynamicsWorld->computeOverlappingPairs();

	//btVector3 red(1, 0, 0);
	//btVector3 blue(0, 0, 1);

	/////all hits
	//{
	//	btVector3 from(-30, 1 + up, 0);
	//	btVector3 to(30, 1, 0);
	//	m_dynamicsWorld->getDebugDrawer()->drawLine(from, to, btVector4(0, 0, 0, 1));
	//	btCollisionWorld::AllHitsRayResultCallback allResults(from, to);
	//	allResults.m_flags |= btTriangleRaycastCallback::kF_KeepUnflippedNormal;
	//	//kF_UseGjkConvexRaytest flag is now enabled by default, use the faster but more approximate algorithm
	//	//allResults.m_flags |= btTriangleRaycastCallback::kF_UseSubSimplexConvexCastRaytest;
	//	allResults.m_flags |= btTriangleRaycastCallback::kF_UseSubSimplexConvexCastRaytest;

	//	m_dynamicsWorld->rayTest(from, to, allResults);

	//	for (int i = 0; i<allResults.m_hitFractions.size(); i++)
	//	{
	//		btVector3 p = from.lerp(to, allResults.m_hitFractions[i]);
	//		m_dynamicsWorld->getDebugDrawer()->drawSphere(p, 0.1, red);
	//		m_dynamicsWorld->getDebugDrawer()->drawLine(p, p + allResults.m_hitNormalWorld[i], red);
	//	}
	//}

	/////first hit

	/*btVector3 from(ray.origin.x, ray.origin.y, ray.origin.z);
	btVector3 direction(ray.direction.x, ray.direction.y, ray.direction.z);
	btVector3 to(from + direction * distance);

	btCollisionWorld::ClosestRayResultCallback	closestResults(from, to);
	closestResults.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;

	Application::Instance()->GetPhyscis()->GetPhysicsWorld()->GetBulletDynamicWorld()->rayTest(from, to, closestResults);

	if (closestResults.hasHit())
	{
	btCollisionObject* obj = const_cast<btCollisionObject*>(closestResults.m_collisionObject);
	btRigidBody* body = btRigidBody::upcast(obj);

	int count = pPool->GetComponentCount();

	shared_ptr<RigidBody> rigidBody;
	btVector3 hitPointWorld = closestResults.m_hitPointWorld;
	for (int i = 0; i < count; ++i)
	{
	rigidBody = pPool->GetComponent(i);
	if (body == rigidBody->GetRigidBody().get())
	{
	gameObject = rigidBody->GetGameObject();
	hitPoint = Vector3(hitPointWorld.getX(), hitPointWorld.getY(), hitPointWorld.getZ());
	return true;
	}
	}
	}*/

	return false;
}



