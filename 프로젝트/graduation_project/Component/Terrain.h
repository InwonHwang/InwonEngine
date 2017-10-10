#pragma once

#include "Renderer.h"

class QuadTree;

class Terrain final : public Renderer//, public std::enable_shared_from_this<Terrain>
{
public:
	Terrain()
		:
		Renderer(),
		_terrainData(nullptr),
		_quadTree(nullptr)
	{
	}
	virtual ~Terrain() {}

	virtual void Update(void* pData) override;
	virtual void Create(void* pData, const shared_ptr<void> gameObject) override;
	virtual void Destroy() override;


	void SetTerrainData(const shared_ptr<TerrainData> terrainData);
	shared_ptr<TerrainData> GetTerrainData() const;
	shared_ptr<QuadTree> GetQuadTree() const;

private:
	shared_ptr<TerrainData> _terrainData;
	shared_ptr<QuadTree>	_quadTree;
};

inline shared_ptr<TerrainData> Terrain::GetTerrainData() const
{
	return _terrainData;
}

inline shared_ptr<QuadTree> Terrain::GetQuadTree() const
{
	return _quadTree;
}