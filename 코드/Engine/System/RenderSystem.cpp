#include "RenderSystem.h"
#include "..\Graphics\Graphics.h"
#include "..\Component\Component.h"
#include "..\Resource\Resource.h"


RenderSystem::RenderSystem(shared_ptr<D3D9Core>& d3dCore)
	: _d3dCore(d3dCore)
{
}

RenderSystem::~RenderSystem()
{	
}

void RenderSystem::begin()
{
	D3DXMATRIX m;
	_d3dCore->GetD3DDevice()->SetTransform(D3DTS_WORLD, D3DXMatrixIdentity(&m));
	_d3dCore->GetD3DDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 32, 64, 128), 1.0f, 0);
	_d3dCore->GetD3DDevice()->BeginScene();
}

void RenderSystem::end()
{
	_d3dCore->GetD3DDevice()->EndScene();
	_d3dCore->GetD3DDevice()->Present(NULL, NULL, NULL, NULL);
}

void RenderSystem::Update()
{
	begin();

	drawSkyBox();

	auto rendererObjects = GameObject::FindObjectsOfType<Renderer>();

	for (auto rendererObject : *rendererObjects) {
		auto renderer = rendererObject->GetComponent<Renderer>();

		if (typeid(*renderer) == typeid(SkinnedMeshRenderer)) drawSkinnedMesh(renderer);
		else if (typeid(*renderer) == typeid(MeshRenderer))	drawMesh(renderer);
		else if (typeid(*renderer) == typeid(Terrain))	drawTerrain(renderer);

	}

	end();
	/*BOXVERTEX vtx[8];
	vtx[0] = BOXVERTEX(1, 1, 1, 0xffffffff);
	vtx[1] = BOXVERTEX(1, 1, -1, 0xffffffff);
	vtx[2] = BOXVERTEX(-1, 1, 1, 0xffffffff);
	vtx[3] = BOXVERTEX(-1, 1, -1, 0xffffffff);
	vtx[4] = BOXVERTEX(1, -1, 1, 0xffffffff);
	vtx[5] = BOXVERTEX(1, -1, -1, 0xffffffff);
	vtx[6] = BOXVERTEX(-1, -1, 1, 0xffffffff);
	vtx[7] = BOXVERTEX(-1, -1, -1, 0xffffffff);

	INDEX idx[12]
	{
	{ 0,1,2 },{ 2,1,3 },
	{ 4,0,6 },{ 6,0,2 },
	{ 7,5,6 },{ 6,5,4 },
	{ 3,1,7 },{ 7,1,5 },
	{ 4,5,0 },{ 0,5,1 },
	{ 3,7,2 },{ 2,7,6 }
	};

	auto transformObjects = GameObject::FindObjectsOfType<Transform>();

	for (auto transformObject : *transformObjects) {

	if (transformObject->GetComponent<Renderer>())continue;

	auto transform = transformObject->GetComponent<Transform>();
	auto parent = transform->GetParent();

	auto matWorld = *(transform->GetWorldMatrix());

	_d3dCore->GetD3DDevice()->SetTransform(D3DTS_WORLD, &matWorld);

	_d3dCore->GetD3DDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	_d3dCore->GetD3DDevice()->SetFVF(BOXVERTEX::FVF);
	_d3dCore->GetD3DDevice()->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 8, 12, idx, D3DFMT_INDEX32, vtx, sizeof(BOXVERTEX));
	}*/

	
}

void RenderSystem::drawSkyBox()
{
	auto skyBox = Scenes->GetCurrentScene()->GetSkyBox();

	if (!skyBox) return;

	auto pd3dDevice = _d3dCore->GetD3DDevice();

	IDirect3DVertexBuffer9* vb = skyBox->GetD3D9VertexBuffer();

	pd3dDevice->SetStreamSource(0, vb, 0, skyBox->GetStride());
	pd3dDevice->SetFVF(skyBox->GetFVF());

	pd3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	pd3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	pd3dDevice->SetRenderState(D3DRS_AMBIENT, 0xffffffff);

	D3DMATERIAL9 mtrl;
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
	mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
	
	D3DXMATRIX m;
	D3DXMatrixIdentity(&m);

	pd3dDevice->SetMaterial(&mtrl);
	pd3dDevice->SetTransform(D3DTS_WORLD, &m);

	for (int i = 0; i < 6; i++)
	{
		auto texture = skyBox->GetTexture(i)->GetD3DTexture();

		pd3dDevice->SetTexture(0, texture);
		pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);
	}
}

void RenderSystem::drawMesh(const shared_ptr<Renderer>& renderer)
{
	auto meshRenderer = static_pointer_cast<MeshRenderer>(renderer);

	auto pd3dDevice = _d3dCore->GetD3DDevice();
	auto mesh = meshRenderer->GetMesh();
	int subMeshCount = mesh->GetSubMeshCount();
	
	for (int i = 0; i < subMeshCount; ++i)
	{
		auto subMesh = mesh->GetSubMesh(i);
		auto material = meshRenderer->GetMaterial(i);

		if (!material) continue;

		IDirect3DVertexBuffer9* vb = subMesh->GetD3D9VertexBuffer();
		IDirect3DIndexBuffer9* ib = subMesh->GetD3D9IndexBuffer();

		pd3dDevice->SetStreamSource(0, vb, 0, subMesh->GetStride());
		pd3dDevice->SetFVF(subMesh->GetFVF());
		pd3dDevice->SetIndices(ib);

		auto effect = material->GetEffect()->GetD3DEffect();

		effect->SetBool("IsSkinned", false);
		effect->SetMatrix("WorldMatrix", meshRenderer->GetComponent<Transform>()->GetWorldMatrix().get());
		material->UpdateParam();

		unsigned int numPass = 0;

		effect->Begin(&numPass, 0);

		for (int j = 0; j < numPass; ++j)
		{
			effect->BeginPass(j);

			pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, subMesh->GetVertices()->size(), 0, subMesh->GetIndices()->size());
			effect->EndPass();
		}

		effect->End();
	}
}

void RenderSystem::drawSkinnedMesh(const shared_ptr<Renderer>& renderer)
{
	auto skinnedMeshRenderer = static_pointer_cast<SkinnedMeshRenderer>(renderer);

	auto pd3dDevice = _d3dCore->GetD3DDevice();
	auto mesh = skinnedMeshRenderer->GetMesh();	
	auto bones = skinnedMeshRenderer->GetBones();

	int subMeshCount = mesh->GetSubMeshCount();
	for (int i = 0; i < subMeshCount; ++i)
	{
		auto subMesh = static_pointer_cast<SkinnedMesh>(mesh->GetSubMesh(i));
		auto material = skinnedMeshRenderer->GetMaterial(i);

		if (!material) continue;

		IDirect3DVertexBuffer9* vb = subMesh->GetD3D9VertexBuffer();
		IDirect3DIndexBuffer9* ib = subMesh->GetD3D9IndexBuffer();

		pd3dDevice->SetStreamSource(0, vb, 0, subMesh->GetStride());
		pd3dDevice->SetFVF(subMesh->GetFVF());
		pd3dDevice->SetIndices(ib);

		vector<D3DXMATRIX> matrices(subMesh->GetOffsetMatrices()->begin(), subMesh->GetOffsetMatrices()->end());

		for(int j = 0; j < bones->size(); ++j)
			matrices[j] = matrices[j] * *(bones->data()[j]->GetWorldMatrix());

		auto effect = material->GetEffect()->GetD3DEffect();

		effect->SetBool("IsSkinned", (bones->size() != 0));
		effect->SetMatrixArray("MatrixPalette", matrices.data(), matrices.size());
		effect->SetMatrix("WorldMatrix", skinnedMeshRenderer->GetComponent<Transform>()->GetWorldMatrix().get());
		material->UpdateParam();

		unsigned int numPass = 0;

		effect->Begin(&numPass, 0);

		for (int j = 0; j < numPass; ++j)
		{
			effect->BeginPass(j);

			pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, subMesh->GetVertices()->size(), 0, subMesh->GetIndices()->size());
			effect->EndPass();
		}

		effect->End();
	}
}

void RenderSystem::drawTerrain(const shared_ptr<Renderer>& renderer)
{
	auto terrain = static_pointer_cast<Terrain>(renderer);
	auto d3device = _d3dCore->GetD3DDevice();
	auto terrainData = terrain->GetTerrainData();
	auto quadtree = terrain->GetQuadTree();

	auto indexBuffer = terrainData->GetD3D9IndexBuffer();
	int width = terrainData->GetWidth();
	int primitiveCount = 0;

	INDEX* pI = nullptr;

	if (SUCCEEDED(indexBuffer->Lock(0, (width - 1)*(width - 1) * 2 * sizeof(INDEX), (void**)&pI, 0)))
	{
		primitiveCount = quadtree->GenerateIndex(pI, terrainData->GetVertices(), Camera::mainCamera->GetFrustum(), 0.025f);
		indexBuffer->Unlock();
	}

	IDirect3DVertexBuffer9* d3dvb = terrainData->GetD3D9VertexBuffer();
	IDirect3DIndexBuffer9* d3dib = terrainData->GetD3D9IndexBuffer();

	d3device->SetStreamSource(0, d3dvb, 0, terrainData->GetStride());
	d3device->SetFVF(terrainData->GetFVF());
	d3device->SetIndices(d3dib);

	int materialCount = terrain->GetMaterialCount();

	for (int i = 0; i < materialCount; ++i)
	{
		auto material = terrain->GetMaterial(i);
		auto effect = material->GetEffect()->GetD3DEffect();

		effect->SetMatrix("WorldMatrix", terrain->GetComponent<Transform>()->GetWorldMatrix().get());
		material->UpdateParam();

		unsigned int numPass = 0;

		effect->Begin(&numPass, 0);
		for (int j = 0; j < numPass; ++j)
		{
			effect->BeginPass(j);

			d3device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, terrainData->GetVertices()->size(), 0, primitiveCount);

			effect->EndPass();
		}

		effect->End();
	}
}
