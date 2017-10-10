#include "Camera.h"
#include "Transform.h"



shared_ptr<Camera> Camera::mainCamera;

void Camera::Update(void* pData)
{
	IDirect3DDevice9* pd3dDevice = static_cast<IDirect3DDevice9*>(pData);
	
	_frustum->Make(GetViewProjMatrix());

	shared_ptr<Transform> transform = _gameObject->GetComponent<Transform>();

	Vector3 eye;
	Vector3 lookAt;
	Vector3 up;
	Vector3 axisY;
	Vector3 axisZ(0, 0, 0);

	transform->GetLocalPosition(eye);
	transform->GetAxisY(axisY);
	transform->GetAxisZ(axisZ);

	lookAt = eye + axisZ;
	up = axisY;

	D3DXMatrixLookAtLH(_viewMatrix.get(), &eye, &lookAt, &up);
	D3DXMatrixPerspectiveFovLH(_projectionMatrix.get(), D3DXToRadian(45), 1.0f, 1.0f, 5000.0f);

	//D3DXMatrixOrthoLH(_projectionMatrix, 1, 0.5f, 0.5f, 1000.0f);
	pd3dDevice->SetTransform(D3DTS_VIEW, _viewMatrix.get());
	pd3dDevice->SetTransform(D3DTS_PROJECTION, _projectionMatrix.get());
}

void Camera::Create(void* pData, const shared_ptr<void> gameObject)
{	
	_gameObject = static_pointer_cast<GameObject>(gameObject);
	_viewMatrix.reset(new D3DXMATRIX);
	_projectionMatrix.reset(new D3DXMATRIX);
	D3DXMatrixIdentity(_viewMatrix.get());
	D3DXMatrixIdentity(_projectionMatrix.get());
	_frustum.reset(new Frustum());
}

void Camera::Destroy()
{
	_gameObject.reset();
	_viewMatrix.reset();
	_projectionMatrix.reset();
	_frustum.reset();
}

RAY Camera::ScreenPointToRay(const Vector2& mousePosition)
{
	D3DXMATRIX world, view, proj, matInverse;
	world = *_gameObject->GetComponent<Transform>()->GetWorldMatrix();
	view = *_viewMatrix;
	proj = *_projectionMatrix;

	float xAngle = (((2.0f * mousePosition.x) / 800) - 1.0f) / proj._11;
	float yAngle = -(((2.0f * mousePosition.y) / 600) - 1.0f) / proj._22;

	Vector3 direction;
	direction = Vector3(xAngle, yAngle, 1.0f);

	D3DXMatrixInverse(&matInverse, NULL, &(view));
	D3DXVec3TransformNormal(&direction, &direction, &matInverse);
	D3DXVec3Normalize(&direction, &direction);

	Vector3 position;
	_gameObject->GetComponent<Transform>()->GetPosition(position);

	RAY ray;
	ray.origin = position;
	ray.direction = direction;

	return ray;
}
