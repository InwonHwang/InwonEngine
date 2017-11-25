#pragma once
#include "WindowBase.h"

class SceneWindow :	public WindowBase
{
public:
	SceneWindow(const string& name, const ImVec2& pos, const ImVec2& size);
	~SceneWindow();

	virtual void Update() override;
};

