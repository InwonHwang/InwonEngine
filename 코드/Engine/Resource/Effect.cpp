#include "Effect.h"

Effect::Effect()
	: ResourceBase(),
	_pEffect(nullptr)
{
	_name = make_shared<string>();
}

Effect::~Effect()
{
}

bool Effect::Create(void* pData, void* pResourceData, const string& name)
{
	*_name = name;

	IDirect3DDevice9* pd3dDevice = static_cast<IDirect3DDevice9*>(pData);

	if (FAILED(D3DXCreateEffectFromFile(pd3dDevice, _name->c_str(), NULL, NULL, 0, NULL, &_pEffect, NULL)))
		return false;

	return true;
}

void Effect::Destroy()
{
	SafeRelease<ID3DXEffect>(_pEffect);
}