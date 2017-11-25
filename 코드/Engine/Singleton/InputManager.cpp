#include "InputManager.h"

bool InputManager::Init()
{
	return true;
}

void InputManager::Release()
{
}

bool InputManager::GetKeyDown(KeyCode code)
{
	return GetAsyncKeyState(code) & 0x8000;
}

bool InputManager::GetKeyUp(KeyCode code)
{
	return  !(GetAsyncKeyState(code) & 0x8000);
}

bool InputManager::GetMouseButtonDown(MouseCode code)
{
	return GetAsyncKeyState(code) & 0x8000;
}

bool InputManager::GetMouseButtonUp(MouseCode code)
{
	return  !(GetAsyncKeyState(code) & 0x8000);
}

Vector2 InputManager::GetMousePoint()
{
	POINT point;
	GetCursorPos(&point);

	return Vector2(point.x, point.y);
}