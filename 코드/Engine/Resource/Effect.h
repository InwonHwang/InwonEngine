#pragma once

#include "..\Core\Core.h"

class Effect : public ResourceBase
{
private:
	ID3DXEffect* _pEffect;

public:
	Effect();
	~Effect();

	virtual bool Create(void* pData, void* pResourceData, const string& name) override;
	virtual void Destroy() override;

	ID3DXEffect* GetD3DEffect() const { return _pEffect; }
};

