#include "Terrain.h"

Terrain::Terrain(const shared_ptr<GameObject> gameObject)
	: Renderer(gameObject),
	_terrainData(nullptr),
	_quadTree(nullptr)
{
}

Terrain::~Terrain()
{
}


void Terrain::createQuadTree(shared_ptr<TerrainData> terrainData)
{
	int width = terrainData->GetWidth();
	_quadTree.reset(new QuadTree(width, width));
	_quadTree->Build(terrainData->GetVertices());
}

void Terrain::Update(void* pData)
{
}

void Terrain::Create(void* pData)
{
}

void Terrain::Destroy()
{	
	_gameObject.reset();
}