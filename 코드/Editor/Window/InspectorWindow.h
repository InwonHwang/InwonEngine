#pragma once

#include "WindowBase.h"

class InspectorWindow : public WindowBase
{
protected:
	void setModal(const string& path);
	void setDropDown();

	void setTransformView(const shared_ptr<Transform> transform);
	void setSkinnedMeshRendererView(const shared_ptr<SkinnedMeshRenderer> skinnedMeshRenderer);
	void setMeshRendererView(const shared_ptr<MeshRenderer> meshRenderer);
	void setAnimatorView(const shared_ptr<Animator> animator);
	void setRigidBodyView(const shared_ptr<RigidBody> collider);
	void setBoxColliderView(const shared_ptr<BoxCollider> collider);

public:
	InspectorWindow(const string& name, const ImVec2& pos, const ImVec2& size);
	~InspectorWindow();

	virtual void Update() override;
};

