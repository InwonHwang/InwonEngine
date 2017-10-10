#pragma once
#include "..\Core\Core.h"
#include "..\Graphics\Graphics.h"
#include "..\Resource\Resource.h"
#include "Texture.h"

class Material : public ResourceBase
{
	typedef vector<D3DXPARAMETER_DESC> EffectParamVec;

private:
	ID3DXEffect* _pEffect;
	shared_ptr<map<string, bool>>		_booleans;
	shared_ptr<map<string, int>>		_integers;
	shared_ptr<map<string, float>>		_floats;
	shared_ptr<map<string, Vector3>>	_vectors;
	shared_ptr<map<string, Vector4>>	_colors;
	shared_ptr<map<string, shared_ptr<Texture>>> _textures;

public:
	Material() : _pEffect(nullptr) {}
	~Material();

	virtual bool Create(void* pData, void* pResourceData, const string& name) override;
	virtual void Destroy() override;

	ID3DXEffect* GetEffect() const { return _pEffect; }

	void UpdateParam();

	void SetBool(const string& name, bool value);
	void SetInt(const string& name, int value);
	void SetFloat(const string& name, float value);
	void SetVector3(const string& name, const Vector3& value);
	void SetColor(const string& name, const Color& color);
	void SetTexture(const string& name, const shared_ptr<Texture> texture);
};
