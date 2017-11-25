#pragma once

#include "Vector.h"

typedef struct Bounds
{
	shared_ptr<Vector3> center;
	shared_ptr<Vector3> extents;

	Bounds()
		: center(nullptr),
		extents(nullptr)
	{
		center = make_shared<Vector3>(0.0f, 0.0f, 0.0f);
		extents = make_shared<Vector3>(0.0f, 0.0f, 0.0f);
	}

}Bounds;
