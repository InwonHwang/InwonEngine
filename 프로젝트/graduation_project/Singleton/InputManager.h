#pragma once

#include "..\Core\Core.h"

class InputManager : public SingletonBase<InputManager>
{
public:
	enum MouseCode
	{
		eButton_Left = 0x01,
		eButton_Right = 0x02
	};

	enum KeyCode
	{
		eKey_LeftArrow = 0x25,
		eKey_UpArrow = 0x26,
		eKey_RightArrow = 0x27,		
		eKey_DownArrow = 0x28,
		eKey_Num0 = 0x30,
		eKey_Num1 = 0x31,
		eKey_Num2 = 0x32,
		eKey_Num3 = 0x33,
		eKey_Num4 = 0x34,
		eKey_Num5 = 0x35,
		eKey_Num6 = 0x36,
		eKey_Num7 = 0x37,
		eKey_Num8 = 0x38,
		eKey_Num9 = 0x39,
		eKey_A = 0x41,
		eKey_B = 0x42,
		eKey_C = 0x43,
		eKey_D = 0x44,
		eKey_E = 0x45,
		eKey_F = 0x46,
		eKey_G = 0x47,
		eKey_H = 0x48,
		eKey_I = 0x49,
		eKey_J = 0x4A,
		eKey_K = 0x4B,
		eKey_L = 0x4C,
		eKey_M = 0x4D,
		eKey_N = 0x4E,
		eKey_O = 0x4F,
		eKey_P = 0x50,
		eKey_Q = 0x51,
		eKey_R = 0x52,
		eKey_S = 0x53,
		eKey_T = 0x54,
		eKey_U = 0x55,
		eKey_V = 0x56,
		eKey_W = 0x57,
		eKey_X = 0x58,
		eKey_Y = 0x59,
		eKey_Z = 0x5A,
	};
public:
	InputManager() {}
	~InputManager() {}

	bool Init() override;
	void Release() override;

	bool GetKeyDown(KeyCode code);
	bool GetKeyUp(KeyCode code);
	bool GetMouseButtonDown(MouseCode code);
	bool GetMouseButtonUp(MouseCode code);

	Vector2 GetMousePoint();
};

