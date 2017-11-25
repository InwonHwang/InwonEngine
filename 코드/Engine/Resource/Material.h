#pragma once
#include "..\Core\Core.h"
#include "..\Graphics\Graphics.h"
#include "..\Resource\Resource.h"
#include "Texture.h"

class Material : public ResourceBase
{
	typedef vector<D3DXPARAMETER_DESC> EffectParamVec;

private:
	shared_ptr<Effect> _effect;
	unique_ptr<map<string, bool>>		_booleans;
	unique_ptr<map<string, int>>		_integers;
	unique_ptr<map<string, float>>		_floats;
	unique_ptr<map<string, Vector3>>	_vectors;
	unique_ptr<map<string, Vector4>>	_colors;
	unique_ptr<map<string, shared_ptr<Texture>>> _textures;

public:
	Material();
	~Material();

	virtual bool Create(void* pData, void* pResourceData, const string& name) override;
	virtual void Destroy() override;

	void SetEffect(const shared_ptr<Effect> effect) { _effect = effect; }
	const shared_ptr<Effect> GetEffect() const { return _effect; }

	void UpdateParam();

	void SetBool(const string& name, bool value);
	void SetInt(const string& name, int value);
	void SetFloat(const string& name, float value);
	void SetVector3(const string& name, const Vector3& value);
	void SetColor(const string& name, const Color& color);
	void SetTexture(const string& name, const shared_ptr<Texture> texture);
};
