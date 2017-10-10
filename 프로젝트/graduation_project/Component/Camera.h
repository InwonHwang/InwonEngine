#pragma once
#include "..\Core\Core.h"
#include "..\Graphics\Graphics.h"
#include "..\Resource\Resource.h"

class Transform;
class Frustum;

class Camera final : public ComponentBase//, public std::enable_shared_from_this<Camera>
{
public:
	static shared_ptr<Camera> mainCamera;

public:
	Camera()
		:		
		_viewMatrix(nullptr),
		_projectionMatrix(nullptr),
		_frustum(nullptr)
	{
	}
	~Camera() {}

	virtual void Update(void* pData) override;
	virtual void Create(void* pData, const shared_ptr<void> gameObject) override;
	virtual void Destroy() override;

	RAY ScreenPointToRay(const Vector2& mousePosition);

	D3DXMATRIX GetViewProjMatrix() const;
	//text
	shared_ptr<Frustum> GetFrustum() const
	{
		return _frustum;
	}
private:
	shared_ptr<D3DXMATRIX>	_viewMatrix;
	shared_ptr<D3DXMATRIX>	_projectionMatrix;
	shared_ptr<Frustum>		_frustum;
};

inline D3DXMATRIX Camera::GetViewProjMatrix() const
{
	return *_viewMatrix * *_projectionMatrix;
}

