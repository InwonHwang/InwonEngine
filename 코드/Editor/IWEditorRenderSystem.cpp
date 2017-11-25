#include "IWEditorRenderSystem.h"
#include "IWEditor.h"
#include "Window\Window.h"

extern IWEditor editor;

IWEditorRenderSystem::IWEditorRenderSystem(shared_ptr<D3D9Core>& d3d9Core)
	: RenderSystem(d3d9Core)
{
}


IWEditorRenderSystem::~IWEditorRenderSystem()
{
}

void IWEditorRenderSystem::Update()
{
	begin();

	auto scene = Scenes->GetCurrentScene();

	D3D9Device->SetRenderState(D3DRS_ZENABLE, true);
	D3D9Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	D3D9Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	D3D9Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	D3D9Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	D3D9Device->SetRenderState(D3DRS_SCISSORTESTENABLE, true);

	auto sceneWindow = editor.GetWindow("Scene");

	RECT rect;
	rect.left = sceneWindow->GetPos()->x + 10;
	rect.right = sceneWindow->GetPos()->x + sceneWindow->GetSize()->x - 10;
	rect.top = sceneWindow->GetPos()->y + 30;
	rect.bottom = sceneWindow->GetPos()->y + sceneWindow->GetSize()->y - 10;

	D3D9Device->SetScissorRect(&rect);

	if (scene)
	{
		drawSkyBox();

		auto renderObjects = GameObject::FindObjectsOfType<Renderer>();

		for (auto rendererObject : *renderObjects) {
			auto renderer = rendererObject->GetComponent<Renderer>();
			
			if (typeid(*renderer) == typeid(SkinnedMeshRenderer)) drawSkinnedMesh(renderer);
			else if (typeid(*renderer) == typeid(MeshRenderer))	drawMesh(renderer);
			else if (typeid(*renderer) == typeid(Terrain))	drawTerrain(renderer);
		}

		auto colliderObjects = GameObject::FindObjectsOfType<Collider>();

		for (auto colliderObject : *colliderObjects)
			colliderObject->GetComponent<Collider>()->DebugDraw(D3D9Device);
	}

	D3D9Device->SetRenderState(D3DRS_ZENABLE, false);
	D3D9Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	D3D9Device->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
	ImGui::Render();

	end();
}
