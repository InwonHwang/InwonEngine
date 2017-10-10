#include "RenderSystem.h"
#include "..\Graphics\Graphics.h"
#include "..\Component\Component.h"
#include "..\Resource\Resource.h"
#include "..\Singleton\\MaterialManager.h"


bool RenderSystem::_instantiated = false;

RenderSystem::RenderSystem(shared_ptr<D3D9Core>& device) : _device(device), _materialMgr(nullptr)
{
	assert(!RenderSystem::_instantiated);
	RenderSystem::_instantiated = true;

	_materialMgr.reset(new MaterialManager());
}

RenderSystem::~RenderSystem()
{	
	RenderSystem::_instantiated = false;

	_materialMgr.reset();
}

void RenderSystem::Update()
{
	Camera::mainCamera->Update(_device->GetD3DDevice());
	_materialMgr->Update();
	updateAnimator();
	updateCamera();
	updateRenderer();
}

void RenderSystem::updateAnimator()
{
	/*shared_ptr<ComponentPool<Animator>> pPool = _pComponentMgr->GetComponentPool<Animator>();

	size_t animatorCount = pPool->GetComponentCount();

	for (int i = 0; i < animatorCount; ++i)
		pPool->GetComponent(i)->Update(nullptr);*/
}

void RenderSystem::updateCamera()
{
	//shared_ptr<ComponentPool<Camera>> pPool = _pComponentMgr->GetComponentPool<Camera>();

	//size_t cameraCount = pPool->GetComponentCount();

	//for (int i = 0; i < cameraCount; ++i)
	//	pPool->GetComponent(i)->Update(nullptr);
}

void RenderSystem::updateRenderer()
{
	_device->GetD3DDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 32, 64, 128), 1.0f, 0);
	_device->GetD3DDevice()->BeginScene();

	auto rendererObjects = GameObject::FindObjectsOfType<Renderer>();

	for (auto rendererObject : *rendererObjects) {
		auto renderer = rendererObject->GetComponent<Renderer>();

		if (typeid(*renderer) == typeid(SkinnedMeshRenderer)) drawSkinnedMesh(renderer);
		else if (typeid(*renderer) == typeid(MeshRenderer))	drawMesh(renderer);
		else if (typeid(*renderer) == typeid(Terrain))	drawTerrain(renderer);
		
	}

	_device->GetD3DDevice()->EndScene();
	_device->GetD3DDevice()->Present(NULL, NULL, NULL, NULL);
}

void RenderSystem::drawMesh(const shared_ptr<Renderer>& renderer)
{
	auto meshRenderer = static_pointer_cast<MeshRenderer>(renderer);

	auto d3dDevice = _device->GetD3DDevice();
	auto mesh = meshRenderer->GetMesh();
	int subMeshCount = mesh->GetSubMeshCount();
	
	for (int i = 0; i < subMeshCount; ++i)
	{
		auto subMesh = mesh->GetSubMesh(i);
		auto material = meshRenderer->GetMaterial(i);

		if (!material) continue;

		IDirect3DVertexBuffer9* vb = subMesh->GetVertexBuffer();
		IDirect3DIndexBuffer9* ib = subMesh->GetIndexBuffer();

		d3dDevice->SetStreamSource(0, vb, 0, subMesh->GetStride());
		d3dDevice->SetFVF(subMesh->GetFVF());
		d3dDevice->SetIndices(ib);
		int vertexCount = subMesh->GetVerticesCount();
		int primitiveCount = subMesh->GetPrimitiveCount();

		material->GetEffect()->SetBool("IsSkinned", false);
		material->GetEffect()->SetMatrix("WorldMatrix", meshRenderer->GetGameObject()->GetComponent<Transform>()->GetWorldMatrix().get());
		material->UpdateParam();

		unsigned int numPass = 0;

		material->GetEffect()->Begin(&numPass, 0);

		for (int j = 0; j < numPass; ++j)
		{
			material->GetEffect()->BeginPass(j);

			d3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, vertexCount, 0, primitiveCount);
			material->GetEffect()->EndPass();
		}

		material->GetEffect()->End();
	}
}

void RenderSystem::drawSkinnedMesh(const shared_ptr<Renderer>& renderer)
{
	auto skinnedMeshRenderer = static_pointer_cast<SkinnedMeshRenderer>(renderer);

	auto d3dDevice = _device->GetD3DDevice();
	auto mesh = skinnedMeshRenderer->GetMesh();	
	auto bones = skinnedMeshRenderer->GetBones();

	int subMeshCount = mesh->GetSubMeshCount();

	for (int i = 0; i < subMeshCount; ++i)
	{
		auto subMesh = static_pointer_cast<SkinnedMesh>(mesh->GetSubMesh(i));
		auto material = skinnedMeshRenderer->GetMaterial(i);

		if (!material) continue;

		IDirect3DVertexBuffer9* vb = subMesh->GetVertexBuffer();
		IDirect3DIndexBuffer9* ib = subMesh->GetIndexBuffer();

		d3dDevice->SetStreamSource(0, vb, 0, subMesh->GetStride());
		d3dDevice->SetFVF(subMesh->GetFVF());
		d3dDevice->SetIndices(ib);
		int vertexCount = subMesh->GetVerticesCount();
		int primitiveCount = subMesh->GetPrimitiveCount();

		vector<D3DXMATRIX> matrices(subMesh->GetOffsetMatrices()->begin(), subMesh->GetOffsetMatrices()->end());

		int j = 0;
		for (auto transform : *bones)		
			matrices[j++] = matrices[j++] * *(transform->GetLocalMatrix());

		material->GetEffect()->SetBool("IsSkinned", (bones->size() != 0));
		material->GetEffect()->SetMatrixArray("MatrixPalette", matrices.data(), matrices.size());
		material->GetEffect()->SetMatrix("WorldMatrix", skinnedMeshRenderer->GetGameObject()->GetComponent<Transform>()->GetWorldMatrix().get());
		material->UpdateParam();

		unsigned int numPass = 0;

		material->GetEffect()->Begin(&numPass, 0);

		for (int j = 0; j < numPass; ++j)
		{
			material->GetEffect()->BeginPass(j);

			d3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, vertexCount, 0, primitiveCount);
			material->GetEffect()->EndPass();
		}

		material->GetEffect()->End();
	}
}

void RenderSystem::drawTerrain(const shared_ptr<Renderer>& renderer)
{
	auto terrain = static_pointer_cast<Terrain>(renderer);
	auto d3device = _device->GetD3DDevice();
	auto terrainData = terrain->GetTerrainData();
	auto quadtree = terrain->GetQuadTree();

	auto indexBuffer = terrainData->GetIndexBuffer();
	int width = terrainData->GetWidth();
	int primitiveCount = 0;

	INDEX* pI = nullptr;

	if (SUCCEEDED(indexBuffer->Lock(0, (width - 1)*(width - 1) * 2 * sizeof(INDEX), (void**)&pI, 0)))
	{
		primitiveCount = quadtree->GenerateIndex(pI, terrainData->GetVertices(), Camera::mainCamera->GetFrustum(), 0.025f);
		indexBuffer->Unlock();
	}

	IDirect3DVertexBuffer9* d3dvb = terrainData->GetVertexBuffer();
	IDirect3DIndexBuffer9* d3dib = terrainData->GetIndexBuffer();

	d3device->SetStreamSource(0, d3dvb, 0, terrainData->GetStride());
	d3device->SetFVF(terrainData->GetFVF());
	d3device->SetIndices(d3dib);

	int materialCount = terrain->GetMaterialCount();

	for (int i = 0; i < materialCount; ++i)
	{
		auto material = terrain->GetMaterial(i);

		material->GetEffect()->SetMatrix("gWorldMatrix", terrain->GetGameObject()->GetComponent<Transform>()->GetWorldMatrix().get());
		material->UpdateParam();

		unsigned int numPass = 0;

		material->GetEffect()->Begin(&numPass, 0);
		for (int j = 0; j < numPass; ++j)
		{
			material->GetEffect()->BeginPass(j);

			d3device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, terrainData->GetVerticesCount(), 0, primitiveCount);

			material->GetEffect()->EndPass();
		}

		material->GetEffect()->End();
	}
}
