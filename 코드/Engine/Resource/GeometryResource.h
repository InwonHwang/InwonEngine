#pragma once

#include "..\Core\Core.h"

class GeometryResource abstract : public ResourceBase
{
protected:
	shared_ptr<vector<Vector3>>	_vertices;
	shared_ptr<vector<INDEX>>	_indices;

	IDirect3DVertexBuffer9*		_pVertexBuffer;
	IDirect3DIndexBuffer9*		_pIndexBuffer;

public:
	GeometryResource() :
		ResourceBase(),
		_vertices(nullptr),
		_indices(nullptr),
		_pVertexBuffer(nullptr),
		_pIndexBuffer(nullptr)
	{
		_vertices = make_shared<vector<Vector3>>();
		_indices = make_shared<vector<INDEX>>();
	}

	virtual ~GeometryResource() {}

	virtual bool Create(void* pData, void* pResourceData, const string& name) abstract;
	virtual void Destroy() abstract;

	virtual int GetStride() const abstract;
	virtual unsigned long GetFVF() const abstract;

	IDirect3DVertexBuffer9* GetD3D9VertexBuffer() const { return _pVertexBuffer; }
	IDirect3DIndexBuffer9* GetD3D9IndexBuffer() const { return _pIndexBuffer; }

	const shared_ptr<vector<Vector3>> GetVertices() const { return _vertices; }
	const shared_ptr<vector<INDEX>>	GetIndices() const { return _indices; }
};

