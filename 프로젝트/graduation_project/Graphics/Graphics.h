#pragma once

#include "D3D9Core.h"
#include "Frustum.h"
#include "QuadTree.h"

#define D3D9 D3D9Core::Instance()
#define D3D9Device D3D9Core::Instance()->GetD3DDevice()
#define D3D9Sprite D3D9Core::Instance()->GetD3DSprite()
