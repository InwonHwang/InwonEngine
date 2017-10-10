#pragma once

#include "..\Core\Core.h"

class D3D9Core;

class TransformSystem
{
	static bool _instantiated;
private:
	void processSRT();

public:
	TransformSystem(shared_ptr<D3D9Core>& device);
	~TransformSystem();

	void Update();

protected:
	shared_ptr<D3D9Core> _device;
};

