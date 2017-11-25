#include "WindowBase.h"
#include "../stdafx.h"

WindowBase::WindowBase(const string& name)
	: WindowBase(name, ImVec2(0, 0), ImVec2(300, 300), false)
{
}

WindowBase::WindowBase(const string& name, const ImVec2& pos, const ImVec2& size)
	: WindowBase(name, pos, size, false)
{
}

WindowBase::WindowBase(const string& name, const ImVec2& pos, const ImVec2& size, bool isActived)
	: _isActived(isActived),
	_name(make_shared<string>(name)),
	_pos(make_shared<ImVec2>(pos)),
	_size(make_shared<ImVec2>(size))
{
}

WindowBase::~WindowBase()
{
}