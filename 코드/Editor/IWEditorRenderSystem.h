#pragma once

#include "stdafx.h"

class IWEditorRenderSystem : public RenderSystem
{

public:
	IWEditorRenderSystem(shared_ptr<D3D9Core>& d3d9Core);
	~IWEditorRenderSystem();

	virtual void Update() override;
};

