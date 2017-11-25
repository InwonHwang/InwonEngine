#pragma once

#include "WindowBase.h"

class HierarchyWindow : public WindowBase
{
private:
	int _selected;

private:
	void setTreeNodeRecursively(const shared_ptr<GameObject> gameObject, int& index);

public:
	HierarchyWindow(const string& name, const ImVec2& pos, const ImVec2& size);
	~HierarchyWindow();

	virtual void Update() override;
};

