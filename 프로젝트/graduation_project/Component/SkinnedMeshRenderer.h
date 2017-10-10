#pragma once

#include "MeshRenderer.h"

class Transform;

class SkinnedMeshRenderer final : public MeshRenderer
{
	typedef vector<shared_ptr<Transform>> TransformList;

private:
	shared_ptr<TransformList> _bones;

public:
	SkinnedMeshRenderer()
		:
		MeshRenderer(),
		_bones(nullptr)
	{
	}

	~SkinnedMeshRenderer() {}

	virtual void Update(void* pData) override;
	virtual void Create(void* pData, const shared_ptr<void> gameObject) override;
	virtual void Destroy() override;

	void SetRootBone(const shared_ptr<Transform> pRoot);
	shared_ptr<vector<shared_ptr<Transform>>> GetBones() const { return _bones; }


};

