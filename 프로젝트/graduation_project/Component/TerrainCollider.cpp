#include "TerrainCollider.h"
#include "RigidBody.h"
#include "Terrain.h"


void TerrainCollider::Update(void* pData)
{
	Collider::Update(pData);
}

void TerrainCollider::Create(void* pData, const shared_ptr<void> gameObject)
{
	Collider::Create(pData, gameObject);

	shared_ptr<Terrain> pTerrain = _gameObject->GetComponent<Terrain>();

	if (!pTerrain) return;

	shared_ptr<TerrainData> pTerrainData = pTerrain->GetTerrainData();

	int width = pTerrainData->GetWidth();
	shared_ptr<vector<BYTE>> pRawData = pTerrainData->GetRawData();

	float minH = pTerrainData->GetMinHeight();
	float maxH = pTerrainData->GetMaxHeight();

	//_shape.reset(new btHeightfieldTerrainShape(width,
	//											width,
	//											pRawData->data(),
	//											btScalar(1.0f),
	//											btScalar(minH),
	//											btScalar(maxH),
	//											btScalar(1),
	//											PHY_UCHAR,
	//											true));
}
void TerrainCollider::Destroy()
{
	_gameObject.reset();
}

void TerrainCollider::DebugDraw(IDirect3DDevice9* pd3dDevice)
{

}
