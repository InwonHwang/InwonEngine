#include "Material.h"
#include "..\Singleton\Singleton.h"

Material::Material()
	: ResourceBase(),
	_effect(nullptr)
{
	_booleans = make_unique<map<string, bool>>();
	_integers = make_unique<map<string, int>>();
	_floats = make_unique<map<string, float>>();
	_vectors = make_unique<map<string, Vector3>>();
	_colors = make_unique<map<string, Vector4>>();
	_textures = make_unique<map<string, shared_ptr<Texture>>>();
}

Material::~Material()
{
}

bool Material::Create(void* pData, void* pResourceData, const string& name)
{
	IDirect3DDevice9* pd3dDevice = static_cast<IDirect3DDevice9*>(pData);
	string* pPath = static_cast<string*>(pResourceData);
	
	Json::Value json;
	JsonUtility::ReadFromFile(*pPath, json);

	*_name = name;
	_effect = Resources->Create<Effect>(json["shader"].asString());

	D3DXEFFECT_DESC effectDesc = { 0, };
	_effect->GetD3DEffect()->GetDesc(&effectDesc);

	int paramCount = effectDesc.Parameters;

	for (int i = 0; i < paramCount; ++i)
	{
		D3DXHANDLE handle = NULL;
		D3DXPARAMETER_DESC paramDesc = { 0, };

		handle = _effect->GetD3DEffect()->GetParameter(NULL, i);
		_effect->GetD3DEffect()->GetParameterDesc(handle, &paramDesc);

		if (!paramDesc.Semantic) continue;

		string semantic = paramDesc.Semantic;

		if (semantic.compare("BOOL") == 0) _booleans->insert(make_pair<string, bool>(paramDesc.Name, false));
		else if (semantic.compare("INT") == 0) _integers->insert(make_pair<string, int>(paramDesc.Name, 0));
		else if (semantic.compare("FLOAT") == 0) _floats->insert(make_pair<string, float>(paramDesc.Name, 0));
		else if (semantic.compare("VECTOR3") == 0) _vectors->insert(make_pair<string, Vector3>(paramDesc.Name, Vector3(0, 0, 0)));
		else if (semantic.compare("COLOR") == 0) _colors->insert(make_pair<string, Vector4>(paramDesc.Name, Vector4(255, 255, 255, 255)));
		else if (semantic.compare("TEXTURE") == 0) _textures->insert(make_pair<string, shared_ptr<Texture>>(paramDesc.Name, nullptr));
	}	

	for (auto paramKey : json["booleans"].getMemberNames())	
		SetBool(paramKey, json["bools"][paramKey].asBool());

	for (auto paramKey : json["integers"].getMemberNames())
		SetInt(paramKey, json["integers"][paramKey].asInt());

	for (auto paramKey : json["floats"].getMemberNames())
		SetFloat(paramKey, json["floats"][paramKey].asInt());

	for (auto paramKey : json["Vector3s"].getMemberNames())
		SetVector3(paramKey, Vector3(json["Vector3s"][paramKey]["x"].asFloat(),
			json["Vector3s"][paramKey]["y"].asFloat(),
			json["Vector3s"][paramKey]["z"].asFloat()));

	for (auto paramKey : json["Colors"].getMemberNames())
		SetColor(paramKey, Color(json["Colors"][paramKey]["r"].asFloat(),
			json["Colors"][paramKey]["g"].asFloat(),
			json["Colors"][paramKey]["b"].asFloat(),
			json["Colors"][paramKey]["a"].asFloat()));

	for (auto paramKey : json["textures"].getMemberNames())
		SetTexture(paramKey, Resources->Create<Texture>(json["textures"][paramKey].asString()));

	return true;
}

void Material::Destroy()
{
}

void Material::UpdateParam()
{
	for (auto pair : *_booleans)	
		_effect->GetD3DEffect()->SetBool(pair.first.c_str(), pair.second);

	for (auto pair : *_integers)
		_effect->GetD3DEffect()->SetInt(pair.first.c_str(), pair.second);

	for (auto pair : *_floats)
		_effect->GetD3DEffect()->SetBool(pair.first.c_str(), pair.second);

	for (auto pair : *_vectors)
		_effect->GetD3DEffect()->SetValue(pair.first.c_str(), &pair.second, sizeof(Vector3));

	for (auto pair : *_colors)
		_effect->GetD3DEffect()->SetVector(pair.first.c_str(), &pair.second);

	for (auto pair : *_textures) {
		_effect->GetD3DEffect()->SetTexture(pair.first.c_str(), nullptr);

		if (!pair.second) continue;
		
		_effect->GetD3DEffect()->SetTexture(pair.first.c_str(), pair.second->GetD3DTexture());
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
