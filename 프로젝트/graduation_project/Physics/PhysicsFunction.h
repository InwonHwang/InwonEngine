#pragma once

#include "PhysicsWorld.h"


bool Raycast(const RAY& ray, float distance, Vector3& hitPoint, shared_ptr<GameObject>& gameObject);

