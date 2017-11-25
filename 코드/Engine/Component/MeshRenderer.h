#pragma once

#include "Renderer.h"

class MeshRenderer : public Renderer//, public std::enable_shared_from_this<MeshRenderer>
{
protected:
	shared_ptr<Mesh>	_mesh;
	shared_ptr<Bounds>	_bounds;

protected:
	void initBounds(const shared_ptr<Mesh> mesh);

public:
	MeshRenderer(const shared_ptr<GameObject> gameObject);
	virtual ~MeshRenderer();

	virtual void Update(void* pData) override;
	virtual void Create(void* pData) override;
	virtual void Destroy() override;

	virtual shared_ptr<void> Clone() override;
	virtual void FromJson(const Json::Value& json) override;
	virtual void ToJson(Json::Value& json) override;

	void SetMesh(const shared_ptr<Mesh> mesh) { _mesh = mesh; }
	const shared_ptr<Mesh> GetMesh() const { return _mesh; }

	const shared_ptr<Bounds> GetBounds() const { return _bounds; }
};