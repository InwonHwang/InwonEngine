#include "StaticMesh.h"

StaticMesh::StaticMesh()
	: GeometryResource()
{
}

StaticMesh::~StaticMesh()
{
}

bool StaticMesh::Create(void* pData, void* pResourceData, const string& name)
{
	*_name = name;

	IDirect3DDevice9* pd3dDevice = static_cast<IDirect3DDevice9*>(pData);
	Json::Value* json = static_cast<Json::Value*>(pResourceData);

	int indicesCount = (*json)["indices"].size();
	int verticeCount = (*json)["vertices"].size();
	int primitiveCount = indicesCount / 3;

	if (FAILED(pd3dDevice->CreateVertexBuffer(sizeof(STATICMESHVERTEX) * verticeCount,
		0, STATICMESHVERTEX::FVF, D3DPOOL_MANAGED, &_pVertexBuffer, NULL)))
		return false;

	if (FAILED(pd3dDevice->CreateIndexBuffer(sizeof(INDEX) * primitiveCount,
		0, D3DFMT_INDEX32, D3DPOOL_MANAGED, &_pIndexBuffer, 0)))
	{
		SafeRelease<IDirect3DVertexBuffer9>(_pVertexBuffer);
		return false;
	}

	_vertices->resize(verticeCount);
	_indices->resize(primitiveCount);

	void * pBuffer = nullptr;

	if (SUCCEEDED(_pVertexBuffer->Lock(0, verticeCount * sizeof(STATICMESHVERTEX), &pBuffer, 0)))
	{
		STATICMESHVERTEX* pVertice = static_cast<STATICMESHVERTEX*>(pBuffer);
		STATICMESHVERTEX vertex;
		int i = 0;

		for (auto v : (*json)["vertices"])
		{
			vertex.position = Vector3(v["position"]["x"].asFloat(), v["position"]["y"].asFloat(), v["position"]["z"].asFloat());
			vertex.normal = Vector3(v["normal"]["x"].asFloat(), v["normal"]["y"].asFloat(), v["normal"]["z"].asFloat());
			vertex.texCoord = Vector2(v["texture_coordinates"]["x"].asFloat(), v["texture_coordinates"]["y"].asFloat());
			vertex.tangent = Vector3(v["tangent"]["x"].asFloat(), v["tangent"]["y"].asFloat(), v["tangent"]["z"].asFloat());
			vertex.binormal = Vector3(v["binormal"]["x"].asFloat(), v["binormal"]["y"].asFloat(), v["binormal"]["z"].asFloat());

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

	return true;
}


void StaticMesh::Destroy()
{
	SafeRelease<IDirect3DVertexBuffer9>(_pVertexBuffer);
	SafeRelease<IDirect3DIndexBuffer9>(_pIndexBuffer);
}