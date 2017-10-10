#pragma once

#include "..\pch.h"
#include "Math\Math.h"
#include "BitFlag.h"


typedef struct tagLightInfo
{
	flag32 type;
	Vector3 color;
	float intensity;
}LIGHTINFO, *LPLIGHTINFO;

typedef struct tagDirectionalLightInfo : public tagLightInfo
{
}DIRECTIONALLIGHTINFO, *LPDIRECTIONALLIGHTINFO;

typedef struct tagPointLightInfo : public tagDirectionalLightInfo
{
	float range;
}POINTLIGHTINFO, *LPPOINTLIGHTINFO;

typedef struct tagSpotLightInfo : public tagPointLightInfo
{
	float angle;
}SPOTLIGHTINFO, *LPSPOTLIGHTINFO;

//
class Texture;

typedef struct tagSpriteInfo
{
	shared_ptr<Texture> texture;
	float left;
	float right;
	float top;
	float bottom;
}SPRITEINFO, *LPSPRITEINFO;
