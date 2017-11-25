#pragma once

#include "../stdafx.h"

struct ImVec2;

class WindowBase abstract
{
protected:
	bool _isActived;
	shared_ptr<string> _name;
	shared_ptr<ImVec2> _pos;
	shared_ptr<ImVec2> _size;

protected:
	void link(shared_ptr<WindowBase> win) {}

public:
	WindowBase(const string& name);
	WindowBase(const string& name, const ImVec2& pos, const ImVec2& size);
	WindowBase(const string& name, const ImVec2& pos, const ImVec2& size, bool isActived);
	
	~WindowBase();

	void SetSize(const ImVec2& size) { *_size = size; }
	void SetPos(const ImVec2& pos) { *_pos = pos; }

	const shared_ptr<ImVec2> GetSize() const { return _size; }
	const shared_ptr<ImVec2> GetPos() const { return _pos; }

	void SetActive(bool active) { _isActived = active; }
	bool IsActived() const { return _isActived; }

	void SetName(const string& name) { *_name = name; }
	const shared_ptr<string> GetName() const { return _name; }

	virtual void Update() abstract;
};

