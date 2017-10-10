#pragma once

#include "Renderer.h"

class MeshRenderer : public Renderer//, public std::enable_shared_from_this<MeshRenderer>
{
public:
	MeshRenderer()
		:
		Renderer(),
		_meshContainer(nullptr)
	{
	}
	virtual ~MeshRenderer() {}

	virtual void Update(void* pData) override;
	virtual void Create(void* pData, const shared_ptr<void> gameObject) override;
	virtual void Destroy() override;

	shared_ptr<Mesh> GetMesh() const;
	void SetMesh(shared_ptr<Mesh> pMesh);

protected:
	shared_ptr<Mesh> _meshContainer;
};

inline shared_ptr<Mesh> MeshRenderer::GetMesh() const
{
	return _meshContainer;
}