#pragma once

#include "..\Core\Core.h"

class D3D9Core;

class MaterialSystem
{
private:
	shared_ptr<D3D9Core> _d3dCore;

private:
	void updateCamera();
	void updateLight();

public:
	MaterialSystem(const shared_ptr<D3D9Core> d3dCore);
	~MaterialSystem();

	void Update();
};

