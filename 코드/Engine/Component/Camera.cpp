#include "Camera.h"
#include "Transform.h"

shared_ptr<Camera> Camera::mainCamera;

Camera::Camera(const shared_ptr<GameObject> gameObject)
	: ComponentBase(gameObject),
	_viewMatrix(nullptr),
	_projectionMatrix(nullptr),
	_frustum(nullptr)
{
	_frustum.reset(new Frustum());
	_viewMatrix.reset(new D3DXMATRIX);
	_projectionMatrix.reset(new D3DXMATRIX);

	D3DXMatrixIdentity(_viewMatrix.get());
	D3DXMatrixIdentity(_projectionMatrix.get());
}

Camera::~Camera()
{
}

void Camera::Update(void* pData)
{
}

void Camera::Create(void* pData)
{	
}

void Camera::Destroy()
{
}

shared_ptr<void> Camera::Clone()
{
	return nullptr;
}

void Camera::FromJson(const Json::Value& json)
{
}

void Camera::ToJson(Json::Value& json)
{
}

RAY Camera::ScreenPointToRay(const Vector2& mousePosition)
{
	D3DXMATRIX world, view, proj, matInverse;
	world = *GetComponent<Transform>()->GetWorldMatrix();
	view = *_viewMatrix;
	proj = *_projectionMatrix;

	float xAngle = (((2.0f * mousePosition.x) / 800) - 1.0f) / proj._11;
	float yAngle = -(((2.0f * mousePosition.y) / 600) - 1.0f) / proj._22;

	Vector3 direction;
	direction = Vector3(xAngle, yAngle, 1.0f);

	D3DXMatrixInverse(&matInverse, NULL, &(view));
	D3DXVec3TransformNormal(&direction, &direction, &matInverse);
	D3DXVec3Normalize(&direction, &direction);

	RAY ray;
	ray.origin = *GetComponent<Transform>()->GetPosition();
	ray.direction = direction;

	return ray;
}
