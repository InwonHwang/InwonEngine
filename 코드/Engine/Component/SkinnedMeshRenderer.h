#pragma once

#include "MeshRenderer.h"

class Transform;

class SkinnedMeshRenderer final : public MeshRenderer
{
	typedef vector<shared_ptr<Transform>> TransformVec;

private:
	shared_ptr<TransformVec> _bones;
	shared_ptr<Transform>	 _rootBone;

private:
	void setBoneRecursively(shared_ptr<Transform> rootBone);

public:
	SkinnedMeshRenderer(const shared_ptr<GameObject> gameObject);
	~SkinnedMeshRenderer();

	virtual void Update(void* pData) override;
	virtual void Create(void* pData) override;
	virtual void Destroy() override;

	virtual shared_ptr<void> Clone() override;
	virtual void FromJson(const Json::Value& json) override;
	virtual void ToJson(Json::Value& json) override;

	void SetRootBone(const shared_ptr<Transform> rootBone) { _rootBone = rootBone; setBoneRecursively(rootBone); }
	const shared_ptr<Transform> GetRootBone() const { return _rootBone; }

	void SetBones(const shared_ptr<vector<shared_ptr<Transform>>> bones) { _bones = bones; }
	const shared_ptr<TransformVec> GetBones() const { return _bones; }
};

