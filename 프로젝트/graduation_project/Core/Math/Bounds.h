#pragma once

#include "Vector.h"

typedef struct Bounds
{
	shared_ptr<Vector3> center;
	shared_ptr<Vector3> extents;
}Bounds;
