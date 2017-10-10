#include "Type.h"


map<string, function<void*()>> TypeInfo::creator;
map<string, function<void(void*)>> TypeInfo::destroyer;

namespace std
{
	shared_ptr<vector<string>> split(const string& str, const char token)
	{
		shared_ptr<vector<string>> output(new vector<string>);
		string::size_type prev_pos = 0, pos = 0;

		while ((pos = str.find(token, pos)) != string::npos)
		{
			output->push_back(str.substr(prev_pos, pos - prev_pos));

			prev_pos = ++pos;
		}

		output->push_back(str.substr(prev_pos, pos - prev_pos));

		return output;
	}
}