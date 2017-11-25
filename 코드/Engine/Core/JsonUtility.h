#pragma once
#include "..\stdafx.h"

class JsonUtility abstract
{
public:
	static void WriteToFile(const string& path, const Json::Value& json)
	{
		ofstream stream(path, ios::trunc);

		Json::StyledStreamWriter writer;
		writer.write(stream, json);

		stream.close();
	}

	static bool ReadFromFile(const string& path, Json::Value& json)
	{
		Json::Reader reader;
		ifstream stream(path);
		bool ret = reader.parse(stream, json, false);

		stream.close();

		return ret;
	}
};
