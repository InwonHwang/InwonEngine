#pragma once

#include "WindowBase.h"

class FBXParser;

class ProjectWindow : public WindowBase
{
private:
	int _selected;
	unique_ptr<FBXParser> _fbxParser;

private:
	void setDirectoryList(const experimental::filesystem::path& dir);
	void setFileList();

public:
	ProjectWindow(const string& name, const ImVec2& pos, const ImVec2& size);
	~ProjectWindow();

	virtual void Update() override;
};



