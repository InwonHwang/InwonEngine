#pragma once

#include "Renderer.h"

class QuadTree;

class Terrain final : public Renderer//, public std::enable_shared_from_this<Terrain>
{
private:
	shared_ptr<TerrainData> _terrainData;
	shared_ptr<QuadTree>	_quadTree;

private:
	void createQuadTree(shared_ptr<TerrainData> terrainData);

public:
	Terrain(const shared_ptr<GameObject> gameObject);
	~Terrain();

	virtual void Update(void* pData) override;
	virtual void Create(void* pData) override;
	virtual void Destroy() override;

	virtual shared_ptr<void> Clone() override { return nullptr; }
	virtual void FromJson(const Json::Value& json) override {}
	virtual void ToJson(Json::Value& json) override {}

	void SetTerrainData(const shared_ptr<TerrainData> data) { _terrainData = data; createQuadTree(data); }
	const shared_ptr<TerrainData> GetTerrainData() const { return _terrainData; }
	const shared_ptr<QuadTree> GetQuadTree() const { return _quadTree; }
};