#include "Material.h"
#include "..\Singleton\Singleton.h"

Material::~Material()
{
}

bool Material::Create(void* pData, void* pResourceData, const string& name)
{
	IDirect3DDevice9* pd3dDevice = static_cast<IDirect3DDevice9*>(pData);
	string* pPath = static_cast<string*>(pResourceData);
	_name = make_shared<string>(name);

	Json::Value json;
	Json::Reader reader;
	ifstream stream(*pPath);
	reader.parse(stream, json, false);

	stream.close();

	string path = string("Assets\\effect\\") + json["shader"].asCString();

	if (FAILED(D3DXCreateEffectFromFile(pd3dDevice, path.c_str(), NULL, NULL, 0, NULL, &_pEffect, NULL)))
		return false;

	_booleans = std::make_shared<map<string, bool>>();
	_integers = std::make_shared<map<string, int>>();
	_floats = make_shared<map<string, float>>();
	_vectors = make_shared<map<string, Vector3>>();
	_colors = make_shared<map<string, Vector4>>();
	_textures = make_shared<map<string, shared_ptr<Texture>>>();

	D3DXEFFECT_DESC effectDesc = { 0, };
	_pEffect->GetDesc(&effectDesc);

	int paramCount = effectDesc.Parameters;

	for (int i = 0; i < paramCount; ++i)
	{
		D3DXHANDLE handle = NULL;
		D3DXPARAMETER_DESC paramDesc = { 0, };

		handle = _pEffect->GetParameter(NULL, i);
		_pEffect->GetParameterDesc(handle, &paramDesc);

		if (!paramDesc.Semantic) continue;

		string semantic = paramDesc.Semantic;

		if (semantic.compare("BOOL") == 0) _booleans->insert(make_pair<string, bool>(paramDesc.Name, false));
		else if (semantic.compare("INT") == 0) _integers->insert(make_pair<string, int>(paramDesc.Name, 0));
		else if (semantic.compare("FLOAT") == 0) _floats->insert(make_pair<string, float>(paramDesc.Name, 0));
		else if (semantic.compare("VECTOR3") == 0) _vectors->insert(make_pair<string, Vector3>(paramDesc.Name, Vector3(0, 0, 0)));
		else if (semantic.compare("COLOR") == 0) _colors->insert(make_pair<string, Vector4>(paramDesc.Name, Vector4(255, 255, 255, 255)));
		else if (semantic.compare("TEXTURE") == 0) _textures->insert(make_pair<string, shared_ptr<Texture>>(paramDesc.Name, nullptr));
	}

	for (auto paramKey : json["params"].getMemberNames())
	{
		for (auto key : json["params"][paramKey].getMemberNames())
		{
			if (paramKey.compare("bool") == 0)
			{
				SetBool(key, json["params"][paramKey][key].asBool());
			}
			else if (paramKey.compare("int") == 0)
			{
				SetInt(key, json["params"][paramKey][key].asInt());
			}
			else if (paramKey.compare("float") == 0)
			{
				SetFloat(key, json["params"][paramKey][key].asFloat());
			}
			else if (paramKey.compare("Vector3") == 0)
			{
				SetVector3(key, Vector3(json["params"][paramKey][key]["x"].asFloat(),
					json["params"][paramKey][key]["y"].asFloat(),
					json["params"][paramKey][key]["z"].asFloat()));
			}
			else if (paramKey.compare("Color") == 0)
			{
				SetColor(key, Color(json["params"][paramKey][key]["r"].asFloat(),
					json["params"][paramKey][key]["g"].asFloat(),
					json["params"][paramKey][key]["b"].asFloat(),
					json["params"][paramKey][key]["a"].asFloat()));
			}
			else if (paramKey.compare("Texture") == 0)
			{
				SetTexture(key, Resources->Create<Texture>(json["params"][paramKey][key].asString()));
			}
		}
	}

	return true;
}

void Material::Destroy()
{
	SafeRelease<ID3DXEffect>(_pEffect);
	_booleans.reset();
	_integers.reset();
	_floats.reset();
	_vectors.reset();
	_colors.reset();
	_textures.reset();
}

void Material::UpdateParam()
{
	for (auto pair : *_booleans)	
		_pEffect->SetBool(pair.first.c_str(), pair.second);

	for (auto pair : *_integers)
		_pEffect->SetInt(pair.first.c_str(), pair.second);

	for (auto pair : *_floats)
		_pEffect->SetBool(pair.first.c_str(), pair.second);

	for (auto pair : *_vectors)
		_pEffect->SetValue(pair.first.c_str(), &pair.second, sizeof(Vector3));

	for (auto pair : *_colors)
		_pEffect->SetVector(pair.first.c_str(), &pair.second);

	for (auto pair : *_textures) {
		if(pair.second) _pEffect->SetTexture(pair.first.c_str(), pair.second->GetD3DTexture());
	}
	
}

void Material::SetBool(const string& name, bool value)
{
	if(_booleans->find(name) == _booleans->end()) return;
	
	_booleans->find(name)->second = value;
}

void Material::SetInt(const string& name, int value)
{
	if (_integers->find(name) == _integers->end()) return;
	
	_integers->find(name)->second = value;
}

void Material::SetFloat(const string& name, float value)
{
	if (_floats->find(name) == _floats->end()) return;

	_floats->find(name)->second = value;
}

void Material::SetVector3(const string& name, const Vector3& value)
{
	if (_vectors->find(name) == _vectors->end()) return;

	_vectors->find(name)->second = value;
}

void Material::SetColor(const string& name, const Color& color)
{
	if (_colors->find(name) == _colors->end()) return;

	_colors->find(name)->second = Vector4(color.r, color.g, color.b, color.a);
}

void Material::SetTexture(const string& name, const shared_ptr<Texture> texture)
{
	if (_textures->find(name) == _textures->end()) return;

	_textures->find(name)->second = texture;
}
