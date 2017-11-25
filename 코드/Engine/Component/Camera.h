#pragma once
#include "..\Core\Core.h"
#include "..\Graphics\Graphics.h"
#include "..\Resource\Resource.h"

class Transform;
class Frustum;

class Camera final : public ComponentBase
{
public:
	static shared_ptr<Camera> mainCamera;

private:
	shared_ptr<D3DXMATRIX>	_viewMatrix;
	shared_ptr<D3DXMATRIX>	_projectionMatrix;
	shared_ptr<Frustum>		_frustum;

public:
	Camera(const shared_ptr<GameObject> gameObject);
	~Camera();

	virtual void Update(void* pData) override;
	virtual void Create(void* pData) override;
	virtual void Destroy() override;

	virtual shared_ptr<void> Clone() override;
	virtual void FromJson(const Json::Value& json) override;
	virtual void ToJson(Json::Value& json) override;

	RAY ScreenPointToRay(const Vector2& mousePosition);

	const shared_ptr<D3DXMATRIX> GetViewMatrix() const { return _viewMatrix; }
	const shared_ptr<D3DXMATRIX> GetProjMatrix() const { return _projectionMatrix; }
	const shared_ptr<Frustum> GetFrustum() const { return _frustum;	}
};