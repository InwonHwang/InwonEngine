#pragma once

#include "..\stdafx.h"

class TypeInfo abstract
{
	static map<string, function<void*()>> creator;
	static map<string, function<void(void*)>> destroyer;

public:
	static void RegisterType(const string& className, function<void*()> func0, function<void(void*)> func1)
	{
		TypeInfo::creator.insert(make_pair(className, func0));
		TypeInfo::destroyer.insert(make_pair(className, func1));
	}

	static void* CreateTypeByName(const string& className)
	{
		return TypeInfo::creator[className]();
	}

	static void DestroyTypeByName(const string& className, void* p)
	{
		TypeInfo::destroyer[className](p);
	}
};

#define REGISTER_TYPE(NAME, CREATOR, DESTROYER) TypeInfo::RegisterType(NAME, CREATOR, DESTROYER)
#define CREATE_TYPE_BY_NAME(NAME) TypeInfo::CreateTypeByName(NAME)
#define DESTROY_TYPE_BY_NAME(NAME, PTR) TypeInfo::DestroyTypeByName(NAME, PTR)

namespace std
{
	shared_ptr<vector<string>> split(const string& str, const char token);
}