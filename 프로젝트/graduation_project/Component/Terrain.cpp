#include "Terrain.h"


void Terrain::Update(void* pData)
{
}

void Terrain::Create(void* pData, const shared_ptr<void> gameObject)
{
	_gameObject = static_pointer_cast<GameObject>(gameObject);
	_materials.reset(new MaterialVec);
}

void Terrain::Destroy()
{	
	_gameObject.reset();
	_materials.reset();
	_quadTree.reset();
	_terrainData.reset();
}

void Terrain::SetTerrainData(const shared_ptr<TerrainData> terrainData)
{
	_terrainData = terrainData;
	int width = terrainData->GetWidth();
	_quadTree.reset(new QuadTree(width, width));
	_quadTree->Build(terrainData->GetVertices());
}