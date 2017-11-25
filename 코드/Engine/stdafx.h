#pragma once

#define _USE_MATH_DEFINES

#include <cmath>
#include <float.h>
#include <cassert>
#include <fstream>
#include <algorithm>
#include <functional>
#include <string>
#include <iostream>
#include <fstream>
#include <memory>

#include <bitset>
#include <queue>
#include <list>
#include <vector>
#include <stack>
#include <map>
#include <set>
#include <limits>

using namespace std;

#include <d3d9.h>
#include <d3dx9.h>

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/NarrowPhaseCollision/btRaycastCallback.h>
#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include <boost/pool/pool.hpp>
#include <boost/pool/object_pool.hpp>
#include <boost/pool/pool_alloc.hpp>
#include <boost/pool/singleton_pool.hpp>

#include "json/json.h"