#pragma once

#include "..\Core\Core.h"

class D3D9Core;
class Renderer;
class MaterialManager;

class RenderSystem
{
	static bool _instantiated;
public:
	RenderSystem(shared_ptr<D3D9Core>& device);
	~RenderSystem();

	void Update();
	//
private:
	void drawMesh(const shared_ptr<Renderer>& renderer);
	void drawSkinnedMesh(const shared_ptr<Renderer>& renderer);
	void drawTerrain(const shared_ptr<Renderer>& renderer);

	void updateAnimator();
	void updateCamera();
	void updateRenderer();

protected:
	shared_ptr<D3D9Core>		 _device;
	shared_ptr<MaterialManager>	 _materialMgr;
};