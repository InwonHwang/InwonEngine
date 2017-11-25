#include "SkinnedMesh.h"

SkinnedMesh::SkinnedMesh()
	: GeometryResource(),
	_offsetMatrices(nullptr)
{
	_offsetMatrices = make_shared<vector<D3DXMATRIX>>();
}

SkinnedMesh::~SkinnedMesh()
{
}

bool SkinnedMesh::Create(void* pData, void* pResourceData, const string& name)
{
	IDirect3DDevice9* pd3dDevice = static_cast<IDirect3DDevice9*>(pData);
	Json::Value* json = static_cast<Json::Value*>(pResourceData);

	int indicesCount = (*json)["indices"].size();
	int verticeCount = (*json)["vertices"].size();
	int primitiveCount = indicesCount / 3;
	
	if (FAILED(pd3dDevice->CreateVertexBuffer(sizeof(SKINNEDMESHVERTEX) * verticeCount,
		0, SKINNEDMESHVERTEX::FVF, D3DPOOL_MANAGED, &_pVertexBuffer, NULL))) {
		return false;
	}		

	if (FAILED(pd3dDevice->CreateIndexBuffer(sizeof(INDEX) * primitiveCount,
		0, D3DFMT_INDEX32, D3DPOOL_MANAGED, &_pIndexBuffer, 0)))
	{
		SafeRelease<IDirect3DVertexBuffer9>(_pVertexBuffer);
		return false;
	}

	_vertices->resize(verticeCount);
	_indices->resize(primitiveCount);

	void * pBuffer = nullptr;
	if (SUCCEEDED(_pVertexBuffer->Lock(0, verticeCount * sizeof(SKINNEDMESHVERTEX), &pBuffer, 0)))
	{
		SKINNEDMESHVERTEX* pVertice = static_cast<SKINNEDMESHVERTEX*>(pBuffer);
		SKINNEDMESHVERTEX vertex;
		int i = 0;

		for (auto v : (*json)["vertices"])
		{
			vertex.position = Vector3(v["position"]["x"].asFloat(), v["position"]["y"].asFloat(), v["position"]["z"].asFloat());
			vertex.normal = Vector3(v["normal"]["x"].asFloat(), v["normal"]["y"].asFloat(), v["normal"]["z"].asFloat());
			vertex.texCoord = Vector2(v["texture_coordinates"]["x"].asFloat(), v["texture_coordinates"]["y"].asFloat());
			vertex.tangent = Vector3(v["tangent"]["x"].asFloat(), v["tangent"]["y"].asFloat(), v["tangent"]["z"].asFloat());
			vertex.binormal = Vector3(v["binormal"]["x"].asFloat(), v["binormal"]["y"].asFloat(), v["binormal"]["z"].asFloat());

			int j = 3;
			for (auto bi : v["blending_infos"])
			{
				vertex.index[j] = bi["bone_index"].asFloat();
				vertex.weight[j] = bi["blending_weight"].asFloat(); j--;
			}

			*pVertice++ = vertex;
			_vertices->data()[i++] = vertex.position;
		}

		_pVertexBuffer->Unlock();
	}

	pBuffer = nullptr;
	if (SUCCEEDED(_pIndexBuffer->Lock(0, primitiveCount * sizeof(INDEX), &pBuffer, 0)))
	{
		INDEX* pIndice = static_cast<INDEX*>(pBuffer);
		INDEX index;

		int indicesCount = 0;

		for (auto it = (*json)["indices"].begin(); it != (*json)["indices"].end();)
		{
			index._0 = (*it++).asInt();
			index._2 = (*it++).asInt();
			index._1 = (*it++).asInt();

			*pIndice++ = index;
			_indices->data()[indicesCount++] = index;
		}

		_pIndexBuffer->Unlock();
	}
	
	for (auto matrix : (*json)["offset_matrices"])
	{
		_offsetMatrices->push_back(D3DXMATRIX(
			matrix[0][0].asFloat(), matrix[0][1].asFloat(), matrix[0][2].asFloat(),	matrix[0][3].asFloat(),
			matrix[1][0].asFloat(), matrix[1][1].asFloat(), matrix[1][2].asFloat(),	matrix[1][3].asFloat(),
			matrix[2][0].asFloat(), matrix[2][1].asFloat(), matrix[2][2].asFloat(),	matrix[2][3].asFloat(),
			matrix[3][0].asFloat(), matrix[3][1].asFloat(), matrix[3][2].asFloat(),	matrix[3][3].asFloat()));
	}

	return true;
}

void SkinnedMesh::Destroy()
{
	SafeRelease<IDirect3DVertexBuffer9>(_pVertexBuffer);
	SafeRelease<IDirect3DIndexBuffer9>(_pIndexBuffer);
}