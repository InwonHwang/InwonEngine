#pragma once

#include "..\Core\Core.h"

class D3D9Core;
class Renderer;

class RenderSystem
{
protected:
	shared_ptr<D3D9Core> _d3dCore;

protected:
	void begin();
	void end();

	void drawSkyBox();
	void drawMesh(const shared_ptr<Renderer>& renderer);
	void drawSkinnedMesh(const shared_ptr<Renderer>& renderer);
	void drawTerrain(const shared_ptr<Renderer>& renderer);

public:
	RenderSystem(shared_ptr<D3D9Core>& d3dCore);
	~RenderSystem();

	virtual void Update();
};