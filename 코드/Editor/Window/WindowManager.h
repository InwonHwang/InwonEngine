#pragma once

#include "WindowBase.h"

class WindowManager : public WindowBase
{
	typedef map<string, shared_ptr<WindowBase>> WindowMap;

private:
	unique_ptr<WindowMap> _windows;

public:
	WindowManager(const string& name, const ImVec2& pos, const ImVec2& size);
	~WindowManager();

	void Add(const shared_ptr<WindowBase> win) { _windows->insert(make_pair(*win->GetName(), win)); }
	const shared_ptr<WindowBase> Get(const string& name) const { return _windows->find(name)->second; }
	virtual void Update() override;
};

