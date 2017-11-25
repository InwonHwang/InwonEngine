#pragma once

#include "..\stdafx.h"

class ICloneable abstract
{
public:
	virtual shared_ptr<void> Clone() abstract;
};

class IJsonable abstract
{
public:
	virtual void FromJson(const Json::Value& json) abstract;
	virtual void ToJson(Json::Value& json) abstract;
};

class IExportable abstract : public IJsonable
{
public:
	virtual void Export(const string& path) abstract;
};

class ICloneableAndExportable abstract : public ICloneable, public IExportable
{
};

class ICloneableAndJsonable abstract : public ICloneable, public IJsonable
{
};
