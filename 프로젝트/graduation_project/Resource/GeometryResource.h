#pragma once

#include "..\Core\Core.h"
#include "..\Graphics\Graphics.h"

class GeometryResource abstract : public ResourceBase
{
protected:
	shared_ptr<vector<Vector3>>	_vertices;
	shared_ptr<vector<INDEX>>	_indices;

	IDirect3DVertexBuffer9*		_pVertexBuffer;
	IDirect3DIndexBuffer9*		_pIndexBuffer;

public:
	GeometryResource() :
		_vertices(nullptr),
		_indices(nullptr),
		_pVertexBuffer(nullptr),
		_pIndexBuffer(nullptr) {}	
	virtual ~GeometryResource() {}

	virtual bool Create(void* pData, void* pResourceData, const string& name) abstract;
	virtual void Destroy() abstract;
	virtual int GetStride() const abstract;
	virtual unsigned long GetFVF() const abstract;

	IDirect3DVertexBuffer9* GetVertexBuffer() const { return _pVertexBuffer; }
	IDirect3DIndexBuffer9* GetIndexBuffer() const { return _pIndexBuffer; }

	shared_ptr<vector<Vector3>> GetVertices() const { return _vertices; }
	shared_ptr<vector<INDEX>>	GetIndice() const { return _indices; }

	int GetVerticesCount() const { return _vertices->size(); }
	int GetPrimitiveCount() const { return _indices->size(); }
};

